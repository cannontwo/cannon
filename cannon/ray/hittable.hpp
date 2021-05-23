#pragma once
#ifndef CANNON_RAY_HITTABLE_H
#define CANNON_RAY_HITTABLE_H 
/*!
 * \file cannon/ray/hittable.hpp
 * \brief File containing useful classes for geometry hittable by rays.
 */

#include <memory>

#include <Eigen/Dense>

#include <cannon/utils/class_forward.hpp>

using namespace Eigen;

namespace cannon {
  namespace ray {

    CANNON_CLASS_FORWARD(Ray);
    CANNON_CLASS_FORWARD(Material);
    CANNON_CLASS_FORWARD(Aabb);

    /*!
     * \brief Structure recording ray intersection information.
     */
    struct hit_record {
      Vector3d p; //!< Point where ray hit geometry
      Vector3d normal; //!< Normal at hit point
      std::shared_ptr<Material> mat_ptr; //!< Pointer to material for hit surface

      double t; //!< Distance of hit along ray
      double u; //!< Horizontal surface texture coordinate at hit point
      double v; //!< Vertical surface texture coordinate at hit point

      bool front_face; //!< Whether the ray originated from outside the geometry

      /*!
       * Method to store normal with direction always opposite intersecting ray.
       */
      void set_face_normal(const Ray& r, const Vector3d& outward_normal);
    };

    /*!
     * \brief Abstract class representing a hittable geometry.
     */
    class Hittable {

      public:

        /*!
         * Default constructor
         */
        Hittable() :
          object_to_world_(std::make_shared<Affine3d>(Affine3d::Identity())),
          world_to_object_(std::make_shared<Affine3d>(Affine3d::Identity())) {}

        /*!
         * Constructor initializing transforms.
         */
        Hittable(std::shared_ptr<Affine3d> object_to_world) :
          object_to_world_(object_to_world),
          world_to_object_(std::make_shared<Affine3d>(object_to_world->inverse())) {}

        /*!
         * Method to test whether this geometry is hit by the input ray in
         * world space between distance t_min and t_max.
         *
         * \param r The ray to check for intersection.
         * \param t_min Minimal distance along the ray to register an intersection.
         * \param t_max Maximum distance along the ray to register an intersection.
         * \param rec Hit record in which to put details about the intersection.
         *
         * \returns Whether the ray intersects this geometry.
         */
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const;

        /*!
         * Method to test whether this geometry is hit by the input ray in
         * object space between distance t_min and t_max.
         *
         * \param r The ray to check for intersection.
         * \param t_min Minimal distance along the ray to register an intersection.
         * \param t_max Maximum distance along the ray to register an intersection.
         * \param rec Hit record in which to put details about the intersection.
         *
         * \returns Whether the ray intersects this geometry.
         */
        virtual bool object_space_hit(const Ray& r, double t_min, double t_max, hit_record& rec) const = 0;

        /*!
         * Method to generate world-space bounding box for this geometry between input times.
         *
         * \param time_0 Start time for bounding box.
         * \param time_1 End time for bounding box.
         * \param output_box Place to put created bounding box.
         *
         * \returns Whether bounding box exists for this geometry, allowing for infinite geometry.
         */
        virtual bool bounding_box(double time_0, double time_1, Aabb& output_box) const;

        /*!
         * Method to generate object-space bounding box for this geometry between input times.
         *
         * \param time_0 Start time for bounding box.
         * \param time_1 End time for bounding box.
         * \param output_box Place to put created bounding box.
         *
         * \returns Whether bounding box exists for this geometry, allowing for infinite geometry.
         */
        virtual bool object_space_bounding_box(double time_0, double time_1, Aabb& output_box) const = 0;

      public:
        std::shared_ptr<Affine3d> object_to_world_;
        std::shared_ptr<Affine3d> world_to_object_;

    };

  } // namespace ray
} // namespace cannon

#endif /* ifndef CANNON_RAY_HITTABLE_H */
