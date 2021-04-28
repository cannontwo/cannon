#pragma once
#ifndef CANNON_RAY_SPHERE_H
#define CANNON_RAY_SPHERE_H 

/*!
 * \file cannon/ray/sphere.hpp
 * File containing Sphere class definition.
 */

#include <cannon/ray/hittable.hpp>
#include <cannon/ray/ray.hpp>

namespace cannon {
  namespace ray {

    /*!
     * \brief Class representing a raytraceable sphere.
     */
    class Sphere : public Hittable {
      public:
        /*!
         * Default constructor
         */
        Sphere() {}

        /*!
         * Constructor taking a sphere center and radius.
         */
        Sphere(const Vector3d& center, double r) : center_(center), radius_(r) {}

        /*!
         * Destructor.
         */
        virtual ~Sphere() {}

        /*!
         * Inherited from Hittable.
         */
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;

      public:
        Vector3d center_; //!< Sphere center
        double radius_; //!< Sphere radius

    };

  } // namespace ray
} // namespace cannon

#endif /* ifndef CANNON_RAY_SPHERE_H */
