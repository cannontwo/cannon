#ifndef CANNON_GRAPHICS_DIRECTIONAL_LIGHT_H
#define CANNON_GRAPHICS_DIRECTIONAL_LIGHT_H

#include <imgui.h>

#include <cannon/graphics/light.hpp>

namespace cannon {
  namespace graphics {

    class DirectionalLight : public Light {
      public:
        DirectionalLight() = delete;

        DirectionalLight(Vector4f ambient, Vector4f diffuse, Vector4f specular) :
          Light(ambient, diffuse, specular) {}

        virtual void apply(const std::shared_ptr<geometry::DrawableGeom> geom) const override {
          geom->program->set_uniform("directional_light.ambient", ambient_);
          geom->program->set_uniform("directional_light.diffuse", diffuse_);
          geom->program->set_uniform("directional_light.specular", specular_);
          geom->program->set_uniform("directional_light.direction", direction_);
        }

        void set_direction(const Vector4f& direction) {
          direction_ = direction;
        }
        
        virtual void write_imgui(int idx) {
          if (ImGui::TreeNode((name_ + " " + std::to_string(idx)).c_str())) {
            ImGui::ColorEdit3("color", specular_.data());
            ambient_.head(3) = specular_.head(3) * 0.2;
            diffuse_.head(3) = specular_.head(3) * 0.5;

            ImGui::SliderFloat3("direction", direction_.data(), 0.0, 1.0);
            direction_.head(3).normalize();
            ImGui::TreePop();
          }
        }

      private:
        Vector4f direction_ = Vector4f::UnitZ();

        std::string name_ = std::string("DirectionalLight");

    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_DIRECTIONAL_LIGHT_H */
