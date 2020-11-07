#ifndef CANNON_GRAPHICS_SPOTLIGHT_H
#define CANNON_GRAPHICS_SPOTLIGHT_H

#include <cannon/graphics/light.hpp>
#include <cannon/graphics/projection.hpp>

namespace cannon {
  namespace graphics {

    class Spotlight : public Light {
      public:
        Spotlight() = delete;

        Spotlight(Vector4f ambient, Vector4f diffuse, Vector4f specular, Vector4f direction) :
          Light(ambient, diffuse, specular), direction_(direction) {
            direction_.head(3).normalize(); 
          }

        Spotlight(Vector4f ambient, Vector4f diffuse, Vector4f specular, Vector4f direction,
            float cutoff, float outer_cutoff, float constant, float linear, float quadratic) :
          Light(ambient, diffuse, specular), cutoff_(cutoff),
          outer_cutoff_(outer_cutoff), constant_(constant), linear_(linear),
          quadratic_(quadratic), direction_(direction) {
            direction_.head(3).normalize(); 
          }

        virtual void apply(const std::shared_ptr<geometry::DrawableGeom> geom) const override {
          geom->program->set_uniform("light.ambient", ambient_);
          geom->program->set_uniform("light.diffuse", diffuse_);
          geom->program->set_uniform("light.specular", specular_);

          geom->program->set_uniform("light.position", pos_);
          geom->program->set_uniform("light.direction", direction_);
          geom->program->set_uniform("light.cutoff", std::cos(to_radians(cutoff_)));
          geom->program->set_uniform("light.outer_cutoff", std::cos(to_radians(outer_cutoff_)));

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
          geom->program->set_uniform(preamble + ".direction", direction_);
          geom->program->set_uniform(preamble + ".cutoff", std::cos(to_radians(cutoff_)));
          geom->program->set_uniform(preamble + ".outer_cutoff", std::cos(to_radians(outer_cutoff_)));

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

        virtual void write_imgui(int idx) {
          if (ImGui::TreeNode((name_ + " " + std::to_string(idx)).c_str())) {
            ImGui::ColorEdit3("color", specular_.data());
            ambient_.head(3) = specular_.head(3) * 0.2;
            diffuse_.head(3) = specular_.head(3) * 0.5;

            ImGui::SliderFloat("linear", &linear_, 0.0, 1.0);
            ImGui::SliderFloat("quadratic", &quadratic_, 0.0, 2.0);

            ImGui::SliderFloat("cutoff", &cutoff_, 0.0, 90.0);
            ImGui::SliderFloat("outer cutoff", &outer_cutoff_, cutoff_, 90.0);

            if (outer_cutoff_ < cutoff_)
              outer_cutoff_ = cutoff_;

            ImGui::SliderFloat3("position", pos_.data(), -10.0, 10.0);
            ImGui::SliderFloat3("direction", direction_.data(), 0.0, 1.0);
            direction_.head(3).normalize();
            ImGui::TreePop();
          }
        }

      private:
        float cutoff_ = 12.5;
        float outer_cutoff_ = 17.5;

        float constant_ = 1.0;
        float linear_ = 0.14;
        float quadratic_ = 0.07;

        Vector4f direction_;

        std::string name_ = std::string("Spotlight");

    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_SPOTLIGHT_H */
