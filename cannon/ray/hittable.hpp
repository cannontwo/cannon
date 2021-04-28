#pragma once
#ifndef CANNON_RAY_HITTABLE_H
#define CANNON_RAY_HITTABLE_H 

/*!
 * \file cannon/ray/hittable.hpp
 * Class containing useful classes for geometry hittable by rays.
 */

#include <memory>

#include <cannon/ray/ray.hpp>

namespace cannon {
  namespace ray {

    // Forward declaration
    class Material;

    /*!
     * \brief Structure recording ray intersection information.
     */
    struct hit_record {
      Vector3d p; //!< Point where ray hit geometry
      Vector3d normal; //!< Normal at hit point
      std::shared_ptr<Material> mat_ptr; //!< Pointer to material for hit surface
      double t; //!< Distance of hit along ray
      bool front_face; //!< Whether the ray originated from outside the geometry

      /*!
       * Method to store normal with direction always opposite intersecting ray.
       */
      inline void set_face_normal(const Ray& r, const Vector3d& outward_normal) {
        front_face = r.dir_.dot(outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
      }
    };

    /*!
     * \brief Abstract class representing a hittable geometry.
     */
    class Hittable {

      public:

        /*!
         * Method to test whether this geometry is hit by the input ray between
         * distance t_min and t_max.
         *
         * \param r The ray to check for intersection.
         * \param t_min Minimal distance along the ray to register an intersection.
         * \param t_max Maximum distance along the ray to register an intersection.
         * \param rec Hit record in which to put details about the intersection.
         *
         * \returns Whether the ray intersects this geometry.
         */
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const = 0;
    };

  } // namespace ray
} // namespace cannon

#endif /* ifndef CANNON_RAY_HITTABLE_H */
