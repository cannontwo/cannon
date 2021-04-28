#pragma once
#ifndef CANNON_RAY_CAMERA_H
#define CANNON_RAY_CAMERA_H 

/*!
 * \file cannon/ray/camera.hpp
 * File containing Camera class definition.
 */

#include <Eigen/Dense>

#include <cannon/ray/ray.hpp>

using namespace Eigen;

namespace cannon {
  namespace ray {

    /*!
     * \brief Class representing a camera generating rays into a scene.
     */
    class Camera {
      public:

        /*!
         * Default constructor.
         */
        Camera() {
          auto aspect_ratio = 16.0 / 9.0;
          auto viewport_height = 2.0;
          auto viewport_width = aspect_ratio * viewport_height;
          auto focal_length = 1.0;

          origin_ = Vector3d::Zero();
          horizontal_ << viewport_width,
                        0.0,
                        0.0;
          vertical_ << 0.0,
                      viewport_height,
                      0.0;

          Vector3d depth;
          depth << 0.0,
                   0.0,
                   focal_length;

          lower_left_corner_ = origin_ - horizontal_/2 - vertical_/2 - depth; 
        }

        /*!
         * Get a ray corresponding to a particular direction into the scene.
         *
         * \param u Horizontal component of ray direction along view.
         * \param v Vertical component of ray direction along view.
         *
         * \returns The sampled ray.
         */
        Ray get_ray(double u, double v) const;

      private:
        Vector3d origin_; //!< Camera origin
        Vector3d lower_left_corner_; //!< Lower-left corner of camera view plane
        Vector3d horizontal_; //!< Horizontal extent of camera view plane
        Vector3d vertical_; //!< Vertical extent of camera view plane

    };

  } // namespace ray
} // namespace cannon

#endif /* ifndef CANNON_RAY_CAMERA_H */
