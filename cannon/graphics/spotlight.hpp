#ifndef CANNON_GRAPHICS_SPOTLIGHT_H
#define CANNON_GRAPHICS_SPOTLIGHT_H

#include <cannon/graphics/light.hpp>
#include <cannon/graphics/projection.hpp>

namespace cannon {
  namespace graphics {

    class Spotlight : public Light {
      public:
        Spotlight() = delete;

        Spotlight(Vector4f ambient, Vector4f diffuse, Vector4f specular) :
          Light(ambient, diffuse, specular) {}

        Spotlight(Vector4f ambient, Vector4f diffuse, Vector4f specular,
            float cutoff, float outer_cutoff, float constant, float linear, float quadratic) :
          Light(ambient, diffuse, specular), cutoff_(cutoff),
          outer_cutoff_(outer_cutoff), constant_(constant), linear_(linear),
          quadratic_(quadratic) {}

        virtual void apply(const std::shared_ptr<geometry::DrawableGeom> geom) const override {
          geom->program->set_uniform("light.ambient", ambient_);
          geom->program->set_uniform("light.diffuse", diffuse_);
          geom->program->set_uniform("light.specular", specular_);

          geom->program->set_uniform("light.position", pos_);
          geom->program->set_uniform("light.cutoff", cutoff_);
          geom->program->set_uniform("light.outer_cutoff", outer_cutoff_);

          geom->program->set_uniform("light.constant", constant_);
          geom->program->set_uniform("light.linear", linear_);
          geom->program->set_uniform("light.quadratic", quadratic_);
        }

        void apply(const std::shared_ptr<geometry::DrawableGeom> geom, int idx) const {
          std::string preamble("spotlights[");
          preamble += std::to_string(idx);
          preamble += "]";

          geom->program->set_uniform(preamble + ".ambient", ambient_);
          geom->program->set_uniform(preamble + ".diffuse", diffuse_);
          geom->program->set_uniform(preamble + ".specular", specular_);

          geom->program->set_uniform(preamble + ".position", pos_);
          geom->program->set_uniform(preamble + ".cutoff", cutoff_);
          geom->program->set_uniform(preamble + ".outer_cutoff", outer_cutoff_);

          geom->program->set_uniform(preamble + ".constant", constant_);
          geom->program->set_uniform(preamble + ".linear", linear_);
          geom->program->set_uniform(preamble + ".quadratic", quadratic_);
        }

        void set_cutoff(float cutoff) {
          cutoff_ = cutoff;
        }

        void set_outer_cutoff(float cutoff) {
          outer_cutoff_ = cutoff;
        }

      private:
        float cutoff_ = std::cos(to_radians(12.5));
        float outer_cutoff_ = std::cos(to_radians(17.5));

        float constant_ = 1.0;
        float linear_ = 0.14;
        float quadratic_ = 0.07;

    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_SPOTLIGHT_H */
