#pragma once
#ifndef CANNON_GRAPHICS_SPOTLIGHT_H
#define CANNON_GRAPHICS_SPOTLIGHT_H

/*!
 * \file cannon/graphics/spotlight.hpp
 * \brief File containing Spotlight class definition.
 */

#include <cannon/graphics/light.hpp>
#include <cannon/graphics/projection.hpp>

namespace cannon {
  namespace graphics {

    /*!
     * \brief Class representing a spotlight which casts light in only a
     * prescribed cone.
     */
    class Spotlight : public Light {
      public:
        Spotlight() = delete;

        /*!  
         * Constructor taking ambient, diffuse, and specular color, as well
         * as direction of spotlight.
         */
        Spotlight(Vector4f ambient, Vector4f diffuse, Vector4f specular, Vector4f direction) :
          Light(ambient, diffuse, specular), direction_(direction) {
            direction_.head(3).normalize(); 
          }

        /*!
         * Constructor taking ambient, diffuse, and specular color, as well as
         * spotlight direction, cutoff and outer cutoff for soft edges, and
         * light falloff parameters.
         */
        Spotlight(Vector4f ambient, Vector4f diffuse, Vector4f specular, Vector4f direction,
            float cutoff, float outer_cutoff, float constant, float linear, float quadratic) :
          Light(ambient, diffuse, specular), cutoff_(cutoff),
          outer_cutoff_(outer_cutoff), constant_(constant), linear_(linear),
          quadratic_(quadratic), direction_(direction) {
            direction_.head(3).normalize(); 
          }

        /*!
         * Inherited from Light.
         */
        virtual void apply(const std::shared_ptr<geometry::DrawableGeom> geom) const override {
          geom->program->set_uniform("light.ambient", (Vector4f)(ambient_ * std::pow(2.0, intensity_)));
          geom->program->set_uniform("light.diffuse", (Vector4f)(diffuse_ * std::pow(2.0, intensity_)));
          geom->program->set_uniform("light.specular", (Vector4f)(specular_ * std::pow(2.0, intensity_)));

          geom->program->set_uniform("light.position", pos_);
          geom->program->set_uniform("light.direction", direction_);
          geom->program->set_uniform("light.cutoff", std::cos(to_radians(cutoff_)));
          geom->program->set_uniform("light.outer_cutoff", std::cos(to_radians(outer_cutoff_)));

          geom->program->set_uniform("light.constant", constant_);
          geom->program->set_uniform("light.linear", linear_);
          geom->program->set_uniform("light.quadratic", quadratic_);
        }
        
        /*!
         * Apply this light to a geometry with a given light index.
         *
         * \param geom Geometry to apply light to.
         * \param idx Index of this light in light collection.
         */
        void apply(const std::shared_ptr<geometry::DrawableGeom> geom, int idx) const {
          apply(geom->program, idx);
        }

        /*!
         * Apply this light to a shader with a given light index.
         *
         * \param s The shader to apply this light to.
         * \param idx Index of this light in light collection.
         */
        void apply(const std::shared_ptr<ShaderProgram> s, int idx) const {
          std::string preamble("spotlights[");
          preamble += std::to_string(idx);
          preamble += "]";

          s->set_uniform(preamble + ".ambient", (Vector4f)(ambient_ * std::pow(2.0, intensity_)));
          s->set_uniform(preamble + ".diffuse", (Vector4f)(diffuse_ * std::pow(2.0, intensity_)));
          s->set_uniform(preamble + ".specular", (Vector4f)(specular_ * std::pow(2.0, intensity_)));

          s->set_uniform(preamble + ".position", pos_);
          s->set_uniform(preamble + ".direction", direction_);
          s->set_uniform(preamble + ".cutoff", std::cos(to_radians(cutoff_)));
          s->set_uniform(preamble + ".outer_cutoff", std::cos(to_radians(outer_cutoff_)));

          s->set_uniform(preamble + ".constant", constant_);
          s->set_uniform(preamble + ".linear", linear_);
          s->set_uniform(preamble + ".quadratic", quadratic_);
        }

        /*!
         * Set inner cutoff for this light.
         *
         * \param cutoff New inner cutoff.
         */
        void set_cutoff(float cutoff) {
          cutoff_ = cutoff;
        }

        /*!
         * Set out cutoff for this light.
         *
         * \param cutoff New outer cutoff.
         */
        void set_outer_cutoff(float cutoff) {
          outer_cutoff_ = cutoff;
        }

        /*!
         * Write this light's parameters to ImGui controls.
         *
         * \param idx Index of this light to disambiguate controls.
         */
        virtual void write_imgui(int idx) {
          if (ImGui::TreeNode((name_ + " " + std::to_string(idx)).c_str())) {
            ImGui::ColorEdit3("color", specular_.data());
            ambient_.head(3) = specular_.head(3) * 0.2;
            diffuse_.head(3) = specular_.head(3) * 0.5;

            ImGui::SliderFloat("intensity", &intensity_, 0.0, 10.0);

            ImGui::SliderFloat("linear", &linear_, 0.0, 1.0);
            ImGui::SliderFloat("quadratic", &quadratic_, 0.0, 2.0);

            ImGui::SliderFloat("cutoff", &cutoff_, 0.0, 90.0);
            ImGui::SliderFloat("outer cutoff", &outer_cutoff_, cutoff_, 90.0);

            if (outer_cutoff_ < cutoff_)
              outer_cutoff_ = cutoff_;

            ImGui::SliderFloat3("position", pos_.data(), -10.0, 10.0);
            ImGui::SliderFloat3("direction", direction_.data(), -1.0, 1.0);
            direction_.head(3).normalize();
            ImGui::TreePop();
          }
        }

      private:
        float cutoff_ = 12.5; //!< Inner cutoff for soft edges of spotlight
        float outer_cutoff_ = 17.5; //!< Outer cutoff for soft edges of spotlight

        float constant_ = 1.0; //!< Constant term in light falloff computation
        float linear_ = 0.14; //!< Linear term in light falloff computation
        float quadratic_ = 0.07; //!< Quadratic term in light falloff computation

        Vector4f direction_; //!< Direction for this spotlight

        std::string name_ = std::string("Spotlight"); //!< Name of this light for ImGui purposes

    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_SPOTLIGHT_H */
