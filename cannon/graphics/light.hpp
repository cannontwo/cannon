#pragma once
#ifndef CANNON_GRAPHICS_LIGHT_H
#define CANNON_GRAPHICS_LIGHT_H 

/*!
 * \file cannon/graphics/light.hpp
 * \brief File containing Light interface definition.
 */

#include <Eigen/Dense>

#include <cannon/graphics/geometry/drawable_geom.hpp>

using namespace Eigen;

namespace cannon {
  namespace graphics {

    /*!
     * \brief Interface unifying all light classes currently implemented.
     *
     * This class is used to abstract over specific light types and provide
     * common method implementations.
     *
     * \sa cannon::graphics::PointLight, cannon::graphics::Spotlight, cannon::graphics::DirectionalLight
     */
    class Light {
      public:
        Light() = delete;

        /*!
         * Constructor taking an ambient, diffuse, and specular color.
         *
         * \param ambient Ambient light color.
         * \param diffuse Diffuse light color.
         * \param specular Specular light color.
         */
        Light(Vector4f ambient, Vector4f diffuse, Vector4f specular) :
          ambient_(ambient), diffuse_(diffuse), specular_(specular) {}

        /*!
         * Empty destructor.
         */
        virtual ~Light() {};

        /*!
         * Method to apply a generic light to a drawable geometry object. This
         * method should be overridden by all children of this class.
         *
         * \param geom The geometry that this light should be applied to.
         */
        virtual void apply(const std::shared_ptr<geometry::DrawableGeom> geom) const {
          geom->program->set_uniform("light.ambient", (Vector4f)(ambient_ * std::pow(2.0, intensity_)));
          geom->program->set_uniform("light.diffuse", (Vector4f)(diffuse_ * std::pow(2.0, intensity_)));
          geom->program->set_uniform("light.specular", (Vector4f)(specular_ * std::pow(2.0, intensity_)));
          geom->program->set_uniform("light.position", pos_);
        }

        /*!
         * Method to set the ambient color of this light.
         *
         * \param ambient The new ambient color.
         */
        void set_ambient(const Vector4f& ambient) {
          ambient_ = ambient;
        }
        
        /*!
         * Method to set the diffuse color of this light.
         *
         * \param diffuse The new diffuse color.
         */
        void set_diffuse(const Vector4f& diffuse) {
          diffuse_ = diffuse;
        }

        /*!
         * Method to set the specular color of this light.
         *
         * \param specular The new specular color.
         */
        void set_specular(const Vector4f& specular) {
          specular_ = specular;
        }

        /*!
         * Method to set the position of this light.
         *
         * \param pos The new position.
         */
        void set_pos(const Vector4f& pos) {
          pos_ = pos;
        }

      protected:
        Vector4f ambient_; //!< The ambient color of this light.
        Vector4f diffuse_; //!< The diffuse color of this light.
        Vector4f specular_; //!< The specular color of this light.
        Vector4f pos_ = Vector4f::Zero(); //!< The position of this light.

        float intensity_ = 0.0; //!< The intensity of this light, used to scale color components.

    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_LIGHT_H */
