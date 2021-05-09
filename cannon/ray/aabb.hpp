#pragma once
#ifndef CANNON_RAY_AABB_H
#define CANNON_RAY_AABB_H 

/*!
 * \file cannon/ray/aabb.hpp
 * File containing Aabb class definition.
 */

#include <cmath>

#include <Eigen/Dense>

#include <cannon/ray/ray.hpp>

using namespace Eigen;

namespace cannon {
  namespace ray {

    /*!
     * \brief Class representing an axis-aligned bounding box.
     */
    class Aabb {
      public:

        /*!
         * Default constructor.
         */
        Aabb() {}

        /*!
         * Constructor taking two points defining this Aabb component-wise.
         */
        Aabb(const Vector3d& a, const Vector3d& b) : minimum_(std::fmin(a.x(),
              b.x()), std::fmin(a.y(), b.y()), std::fmin(a.z(), b.z())),
        maximum_(std::fmax(a.x(), b.x()), std::fmax(a.y(), b.y()),
            std::fmax(a.z(), b.z())) {}

        /*!
         * Test whether a Ray hits this AABB.
         *
         * \param r The ray to test.
         * \param t_min Minimal distance along ray.
         * \param t_max Maximal distance along ray.
         *
         * \returns Whether the ray intersects this AABB between t_min and t_max.
         */
        bool hit(const Ray& r, double t_min, double t_max) const;

      public:
        Vector3d minimum_; //!< Minimal corner of this AABB.
        Vector3d maximum_; //!< Maximal corner of this AABB.

    };

    // Free functions
    
    /*!
     * Function to compute bounding box surrounding both input boxes.
     */
    Aabb surrounding_box(const Aabb& box_0, const Aabb& box_1);

    /*!
     * Function to compute bounding box surrounding input box and an additional point.
     */
    Aabb surrounding_box(const Aabb& box_0, const Vector3d& p);

  } // namespace ray
} // namespace cannon

#endif /* ifndef CANNON_RAY_AABB_H */
