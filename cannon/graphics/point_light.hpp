#ifndef CANNON_GRAPHICS_POINT_LIGHT_H
#define CANNON_GRAPHICS_POINT_LIGHT_H

#include <cannon/graphics/light.hpp>

namespace cannon {
  namespace graphics {

    class PointLight : public Light {
      public:
        PointLight() = delete;

        PointLight(Vector4f ambient, Vector4f diffuse, Vector4f specular) :
          Light(ambient, diffuse, specular) {}

        PointLight(Vector4f ambient, Vector4f diffuse, Vector4f specular,
            float constant, float linear, float quadratic) :
          Light(ambient, diffuse, specular), constant_(constant),
          linear_(linear), quadratic_(quadratic) {}

        virtual void apply(const std::shared_ptr<geometry::DrawableGeom> geom) const override {
          geom->program->set_uniform("light.ambient", ambient_);
          geom->program->set_uniform("light.diffuse", diffuse_);
          geom->program->set_uniform("light.specular", specular_);
          geom->program->set_uniform("light.position", pos_);

          geom->program->set_uniform("light.constant", constant_);
          geom->program->set_uniform("light.linear", linear_);
          geom->program->set_uniform("light.quadratic", quadratic_);
        }

        void apply(const std::shared_ptr<geometry::DrawableGeom> geom, int idx) const {
          std::string preamble("point_lights[");
          preamble += std::to_string(idx);
          preamble += "]";

          geom->program->set_uniform(preamble + ".ambient", ambient_);
          geom->program->set_uniform(preamble + ".diffuse", diffuse_);
          geom->program->set_uniform(preamble + ".specular", specular_);
          geom->program->set_uniform(preamble + ".position", pos_);

          geom->program->set_uniform(preamble + ".constant", constant_);
          geom->program->set_uniform(preamble + ".linear", linear_);
          geom->program->set_uniform(preamble + ".quadratic", quadratic_);
        }

      private:
        float constant_ = 1.0;
        float linear_ = 0.14;
        float quadratic_ = 0.07;

    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_POINT_LIGHT_H */
