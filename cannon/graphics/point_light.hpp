#ifndef CANNON_GRAPHICS_POINT_LIGHT_H
#define CANNON_GRAPHICS_POINT_LIGHT_H

#include <memory>

#include <cannon/graphics/light.hpp>
#include <cannon/graphics/geometry/drawable_geom.hpp>

namespace cannon {
  namespace graphics {

    class PointLight : public Light {
      public:
        PointLight() = delete;

        PointLight(Vector4f ambient, Vector4f diffuse, Vector4f specular,
            std::shared_ptr<geometry::DrawableGeom> cube) : Light(ambient,
            diffuse, specular), cube_(cube) {}

        PointLight(Vector4f ambient, Vector4f diffuse, Vector4f specular,
            std::shared_ptr<geometry::DrawableGeom> cube, float constant, float linear, float quadratic) :
          Light(ambient, diffuse, specular), constant_(constant),
          linear_(linear), quadratic_(quadratic), cube_(cube) {}

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
          apply(geom->program, idx);
        }

        void apply(const std::shared_ptr<ShaderProgram> s, int idx) const {
          std::string preamble("point_lights[");
          preamble += std::to_string(idx);
          preamble += "]";

          s->set_uniform(preamble + ".ambient", ambient_);
          s->set_uniform(preamble + ".diffuse", diffuse_);
          s->set_uniform(preamble + ".specular", specular_);
          s->set_uniform(preamble + ".position", pos_);

          s->set_uniform(preamble + ".constant", constant_);
          s->set_uniform(preamble + ".linear", linear_);
          s->set_uniform(preamble + ".quadratic", quadratic_);
        }

        virtual void write_imgui(int idx) {
          if (ImGui::TreeNode((name_ + " " + std::to_string(idx)).c_str())) {
            ImGui::ColorEdit3("color", specular_.data());
            ambient_.head(3) = specular_.head(3) * 0.2;
            diffuse_.head(3) = specular_.head(3) * 0.5;

            ImGui::SliderFloat("linear", &linear_, 0.0, 1.0);
            ImGui::SliderFloat("quadratic", &quadratic_, 0.0, 2.0);

            ImGui::SliderFloat3("position", pos_.data(), -10.0, 10.0);

            cube_->set_pos(pos_.head(3));
            cube_->set_material({specular_, {0.0, 0.0, 0.0, 1.0}, {0.0, 0.0, 0.0, 1.0}, 32});
            ImGui::TreePop();
          }
        }

      private:
        float constant_ = 1.0;
        float linear_ = 0.14;
        float quadratic_ = 0.07;

        std::shared_ptr<geometry::DrawableGeom> cube_;
        std::string name_ = std::string("Point Light");
    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_POINT_LIGHT_H */
