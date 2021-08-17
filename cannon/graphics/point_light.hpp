#pragma once
#ifndef CANNON_GRAPHICS_POINT_LIGHT_H
#define CANNON_GRAPHICS_POINT_LIGHT_H

/*!
 * \file cannon/graphics/point_light.hpp
 * \brief File containing PointLight class definition.
 */

#include <memory>

#include <imgui.h>

#include <cannon/graphics/light.hpp>
#include <cannon/graphics/geometry/drawable_geom.hpp>

namespace cannon {
  namespace graphics {

    /*!
     * \brief Class representing a point light. 
     *
     * This class embodies the behavior of a point light, which radiates light
     * in all directions. In particular, such a light has ambient, diffuse, and
     * specular colors, as well as a position from which it radiates. The
     * precise behavior of this light is controlled by the shaders used. The
     * position of the point light is represented by a cube.
     */
    class PointLight : public Light {
      public:
        PointLight() = delete;

        /*!
         * Constructor taking an ambient color, diffuse color, specular color,
         * and cube geometry to represent the light's position.
         */
        PointLight(Vector4f ambient, Vector4f diffuse, Vector4f specular,
            std::shared_ptr<geometry::DrawableGeom> cube) : Light(ambient,
            diffuse, specular), cube_(cube) {}

        /*!
         * Constructor taking an ambient color, diffuse color, specular color,
         * cube geometry, and scalar parameters controlling the light's falloff
         * behavior.
         */
        PointLight(Vector4f ambient, Vector4f diffuse, Vector4f specular,
            std::shared_ptr<geometry::DrawableGeom> cube, float constant, float linear, float quadratic) :
          Light(ambient, diffuse, specular), constant_(constant),
          linear_(linear), quadratic_(quadratic), cube_(cube) {}

        /*!
         * Method inherited from Light. Does not change OpenGL state.
         */
        virtual void apply(const std::shared_ptr<geometry::DrawableGeom> geom) const override;

        /*!
         * Variant of the apply method which is useful when this light is one of a collection. 
         *
         * \param geom The geometry to apply this light to.
         * \param idx The index of this light, controlling which shader uniforms are set.
         */
        void apply(const std::shared_ptr<geometry::DrawableGeom> geom, int idx) const {
          apply(geom->program, idx);
        }

        /*!
         * Variant of the apply method which is useful when this light is one
         * of a collection being applied to a shader program.
         *
         * This function does not change OpenGL state.
         *
         * \param s The shader program to apply this light to.
         * \param idx The index of this light, controlling which shader uniforms are set.
         */
        void apply(const std::shared_ptr<ShaderProgram> s, int idx) const;

        /*!
         * Method to write ImGui controls affecting this light's parameters.
         *
         * \param idx The index of this light in a collection of lights, used
         * to disambiguate the corresponding controls.
         */
        virtual void write_imgui(int idx) {
          if (ImGui::TreeNode((name_ + " " + std::to_string(idx)).c_str())) {
            ImGui::ColorEdit3("color", specular_.data());
            ambient_.head(3) = specular_.head(3) * 0.2;
            diffuse_.head(3) = specular_.head(3) * 0.5;

            ImGui::SliderFloat("intensity", &intensity_, 0.0, 10.0);

            ImGui::SliderFloat("linear", &linear_, 0.0, 1.0);
            ImGui::SliderFloat("quadratic", &quadratic_, 0.0, 2.0);

            ImGui::SliderFloat3("position", pos_.data(), -10.0, 10.0);

            cube_->set_pos(pos_.head(3));
            cube_->set_material({specular_, {0.0, 0.0, 0.0, 1.0}, {0.0, 0.0, 0.0, 1.0}, 32});
            ImGui::TreePop();
          }
        }

      private:
        float constant_ = 1.0; //!< Constant term in light intensity falloff
        float linear_ = 0.14; //!< Linear term in light intensity falloff
        float quadratic_ = 0.07; //!< Quadratic term in light intensity falloff

        std::shared_ptr<geometry::DrawableGeom> cube_; //!< Cube geometry for this light
        std::string name_ = std::string("Point Light"); //!< Name for this light
    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_POINT_LIGHT_H */
