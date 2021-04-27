#pragma once
#ifndef CANNON_RAY_RAY_H
#define CANNON_RAY_RAY_H 

/*!
 * \file cannon/ray/ray.hpp
 * File containing Ray class definition.
 */

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace ray {
    
    /*!
     * \brief Class representing a ray for raytracing.
     */
    class Ray {
      public:
        /*!
         * Default constructor.
         */
        Ray() {}

        /*!
         * Constructor taking an origin and direction for this ray.
         */
        Ray(const Vector3d& origin, const Vector3d& direction) : orig_(origin), dir_(direction) {}

        /*!
         * Method computing the location in 3D space that is the input distance
         * along this ray.
         *
         * \param t Distance along ray to compute.
         *
         * \returns Point at distance t along this ray.
         */
        Vector3d at(double t) const;

      public:
        Vector3d orig_; //!< Origin of this ray.
        Vector3d dir_; //!< Direction of this ray.

    };

  }
}

#endif /* ifndef CANNON_RAY_RAY_H */
