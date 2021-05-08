#pragma once
#ifndef CANNON_RAY_HITTABLE_H
#define CANNON_RAY_HITTABLE_H 
/*!
 * \file cannon/ray/hittable.hpp
 * Class containing useful classes for geometry hittable by rays.
 */

#include <memory>

#include <cannon/ray/ray.hpp>
#include <cannon/ray/aabb.hpp>

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
      double u; //!< Horizontal surface texture coordinate at hit point
      double v; //!< Vertical surface texture coordinate at hit point

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
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const { 
          Vector3d object_space_origin = (*world_to_object_) * r.orig_;
          Vector3d object_space_dir = world_to_object_->linear() * r.dir_;

          Ray object_space_ray(object_space_origin, object_space_dir, r.time_);

          if (!object_space_hit(object_space_ray, t_min, t_max, rec))
            return false;

          Vector3d world_space_p = (*object_to_world_) * rec.p;
          Vector3d world_space_normal = object_to_world_->linear() * rec.normal;

          rec.p = world_space_p;
          rec.set_face_normal(object_space_ray, world_space_normal);

          return true;
        }

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
        virtual bool bounding_box(double time_0, double time_1, Aabb& output_box) const {
          Aabb object_space_box;
          bool has_box = object_space_bounding_box(time_0, time_1, object_space_box);

          if (!has_box)
            return false;

          Vector3d min(std::numeric_limits<double>::infinity(),
                       std::numeric_limits<double>::infinity(),
                       std::numeric_limits<double>::infinity());

          Vector3d max(-std::numeric_limits<double>::infinity(),
                       -std::numeric_limits<double>::infinity(),
                       -std::numeric_limits<double>::infinity());

          for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
              for (int k = 0; k < 2; k++) {

                Vector3d corner(i*object_space_box.maximum_.x() + (1-i)*object_space_box.minimum_.x(),
                                j*object_space_box.maximum_.y() + (1-j)*object_space_box.minimum_.y(),
                                k*object_space_box.maximum_.z() + (1-k)*object_space_box.minimum_.z());

                Vector3d trans_corner = (*object_to_world_) * corner;

                for (int c = 0; c < 3; c++) {
                  min[c] = std::fmin(min[c], trans_corner[c]);
                  max[c] = std::fmax(max[c], trans_corner[c]);
                }

              }
            }
          }

          output_box = Aabb(min, max);

          return true;
        }

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
