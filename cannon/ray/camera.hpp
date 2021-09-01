#pragma once
#ifndef CANNON_RAY_CAMERA_H
#define CANNON_RAY_CAMERA_H 

/*!
 * \file cannon/ray/camera.hpp
 * \brief File containing Camera class definition.
 */

#include <Eigen/Dense>

#include <cannon/math/random_double.hpp>
#include <cannon/utils/class_forward.hpp>

using namespace Eigen;

using namespace cannon::math;

namespace cannon {
  namespace ray {

    CANNON_CLASS_FORWARD(Ray);

    /*!
     * \brief Struct representing the sampled random values necessary for
     * generating a camera ray.
     */
    struct CameraSample {
      Vector2d p_film;
      Vector2d p_lens;
      double time;
    };

    /*!
     * \brief Class representing a camera generating rays into a scene.
     */
    class Camera {
      public:

        /*!
         * Constructor taking look point, lookat point, camera up vector, field
         * of view (in radians) and aspect ratio.
         */
        Camera(const Vector3d& look_from, const Vector3d& look_at, const
            Vector3d& vup, double vfov, double aspect_ratio, double aperture,
            double focus_dist, double time_0 = 0.0, double time_1 = 1.0) {
          auto h = std::tan(vfov / 2);
          auto viewport_height = 2.0 * h;
          auto viewport_width = aspect_ratio * viewport_height;

          w_ = (look_from - look_at).normalized();
          u_ = vup.cross(w_).normalized();
          v_ = w_.cross(u_);

          origin_ = look_from;
          horizontal_ = focus_dist * viewport_width * u_;
          vertical_ = focus_dist * viewport_height * v_;
          lower_left_corner_ = origin_ - horizontal_/2 - vertical_/2 - focus_dist * w_; 

          lens_radius_ = aperture / 2;

          time_0_ = time_0;
          time_1_ = time_1;
        }

        /*!
         * Get a ray corresponding to a particular direction into the scene.
         *
         * \param s Horizontal component of ray direction along view.
         * \param t Vertical component of ray direction along view.
         *
         * \returns The sampled ray.
         */
        Ray get_ray(double s, double t) const;

      private:
        Vector3d origin_; //!< Camera origin
        Vector3d lower_left_corner_; //!< Lower-left corner of camera view plane
        Vector3d horizontal_; //!< Horizontal extent of camera view plane
        Vector3d vertical_; //!< Vertical extent of camera view plane
        Vector3d u_, v_, w_; //!< Basis vectors for camera
        double lens_radius_; //!< Simulated thin lens radius
        double time_0_, time_1_; //!< Shutter open and close time
    };

  } // namespace ray
} // namespace cannon

#endif /* ifndef CANNON_RAY_CAMERA_H */
