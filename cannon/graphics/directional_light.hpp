#pragma once
#ifndef CANNON_GRAPHICS_DIRECTIONAL_LIGHT_H
#define CANNON_GRAPHICS_DIRECTIONAL_LIGHT_H

/*!
 * \file cannon/graphics/directional_light.hpp
 * File containing DirectionalLight class definition
 */

#include <imgui.h>

#include <cannon/graphics/light.hpp>

namespace cannon {
  namespace graphics {

    /*!
     * \brief Class representing a directional light source (e.g., the sun)
     *
     * This class really just holds the data necessary to render the effect of
     * a directional light in a scene; no geometry is drawn to represent it. It
     * must be applied to a particular piece of geometry in order for the
     * effects of the light to show up in renders.
     *
     * \sa cannon::graphics::Light, cannon::graphics::PointLight
     *
     */
    class DirectionalLight : public Light {
      public:
        DirectionalLight() = delete;

        /*!
         * Constructor taking the color information for this light.
         *
         * \param ambient The ambient color for this light.
         * \param diffuse The diffuse color for this light.
         * \param specular The specular color for this light.
         */
        DirectionalLight(Vector4f ambient, Vector4f diffuse, Vector4f specular) :
          Light(ambient, diffuse, specular) {}

        /*!  Method to apply this light on a piece of geometry. In effect, sets
         * uniforms on the geometry's associated shader.
         *
         * \param geom The geometry that this light should be drawn on.
         */
        virtual void apply(const std::shared_ptr<geometry::DrawableGeom> geom) const override {
          apply(geom->program);
        }

        /*!
         * Method to apply this light on a shader. In effect, sets uniforms on
         * the shader. This method assumes that only one directional light will
         * be drawn.
         *
         * \param s The shader to apply this light on.
         */
        void apply(const std::shared_ptr<ShaderProgram> s) const {
          s->set_uniform("directional_light.ambient", (Vector4f)(ambient_ * std::pow(2.0, intensity_)));
          s->set_uniform("directional_light.diffuse", (Vector4f)(diffuse_ * std::pow(2.0, intensity_)));
          s->set_uniform("directional_light.specular", (Vector4f)(specular_ * std::pow(2.0, intensity_)));
          s->set_uniform("directional_light.direction", direction_);
        }

        /*!
         * Method to set the direction for this directional light.
         *
         * \param direction The new direction for the light. Should have norm 1.
         */
        void set_direction(const Vector4f& direction) {
          direction_ = direction;
        }
        
        /*!
         * Method to write the imgui controls for this light's parameters. This
         * should be called on each frame render in order for the controls to
         * be displayed consistently.
         *
         * \param idx The index of this light; used to disambiguate different lights.
         */
        virtual void write_imgui(int idx) {
          if (ImGui::TreeNode((name_ + " " + std::to_string(idx)).c_str())) {
            ImGui::ColorEdit3("color", specular_.data());
            ambient_.head(3) = specular_.head(3) * 0.2;
            diffuse_.head(3) = specular_.head(3) * 0.5;

            ImGui::SliderFloat("intensity", &intensity_, 0.0, 10.0);

            ImGui::SliderFloat3("direction", direction_.data(), -1.0, 1.0);
            direction_.head(3).normalize();
            ImGui::TreePop();
          }
        }

        /*!
         * Method to get the current direction of this light.
         *
         * \returns The current direction.
         */
        Vector3f get_dir() {
          return direction_.head(3);
        }

      private:
        Vector4f direction_ = Vector4f::UnitZ(); //!< The current direction for this light.

        std::string name_ = std::string("DirectionalLight"); //!< Name used for ImGui display.

    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_DIRECTIONAL_LIGHT_H */
