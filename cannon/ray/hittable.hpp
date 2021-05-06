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

        /*!
         * Method to generate a bounding box for this geometry between input times.
         *
         * \param time_0 Start time for bounding box.
         * \param time_1 End time for bounding box.
         * \param output_box Place to put created bounding box.
         *
         * \returns Whether bounding box exists for this geometry, allowing for infinite geometry.
         */
        virtual bool bounding_box(double time_0, double time_1, Aabb& output_box) const = 0;
    };

    /*!
     * \brief Class representing translation of a hittable.
     */
    class Translate : public Hittable {
      public:
      
        /*!
         * Constructor taking a hittable to translate and the offset vector to
         * translate by.
         */
        Translate(std::shared_ptr<Hittable> p, const Vector3d& displacement) :
          ptr_(p), offset_(displacement) {}

        /*!
         * Destructor.
         */
        virtual ~Translate() {}

        /*!
         * Inherited from Hittable.
         */
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;

        /*!
         * Inherited from Hittable.
         */
        virtual bool bounding_box(double time_0, double time_1, Aabb& output_box) const override;

      public:
        std::shared_ptr<Hittable> ptr_;
        Vector3d offset_;

    };

    /*!
     * \brief Class representing a rotation of a hittable.
     */
    class Rotate : public Hittable {
      public:

        /*!
         * Constructor taking a hittable to rotate, the axis around which to
         * rotate it, and the angle of rotation. Note that the angle is in radians.
         */
        Rotate(std::shared_ptr<Hittable> p, const Vector3d& axis, double angle)
          : ptr_(p) {

            rotation_ = AngleAxis<double>(angle, axis);
            has_box_ = ptr_->bounding_box(0, 1, bounding_box_);

            Vector3d min(std::numeric_limits<double>::infinity(),
                         std::numeric_limits<double>::infinity(),
                         std::numeric_limits<double>::infinity());

            Vector3d max(-std::numeric_limits<double>::infinity(),
                         -std::numeric_limits<double>::infinity(),
                         -std::numeric_limits<double>::infinity());

            for (int i = 0; i < 2; i++) {
              for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {

                  Vector3d corner(i*bounding_box_.maximum_.x() + (1-i)*bounding_box_.minimum_.x(),
                                  j*bounding_box_.maximum_.y() + (1-j)*bounding_box_.minimum_.y(),
                                  k*bounding_box_.maximum_.z() + (1-k)*bounding_box_.minimum_.z());

                  Vector3d rot_corner = rotation_ * corner;

                  for (int c = 0; c < 3; c++) {
                    min[c] = std::fmin(min[c], rot_corner[c]);
                    max[c] = std::fmax(max[c], rot_corner[c]);
                  }

                }
              }
            }

            bounding_box_ = Aabb(min, max);
        }

        /*!
         * Destructor.
         */
        virtual ~Rotate() {}

        /*!
         * Inherited from Hittable.
         */
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;

        /*!
         * Inherited from Hittable.
         */
        virtual bool bounding_box(double time_0, double time_1, Aabb& output_box) const override {
          output_box = bounding_box_;
          return has_box_;
        }

      public:
        std::shared_ptr<Hittable> ptr_; //!< Hittable being rotated
        bool has_box_; //!< Whether the rotated hittable has a bounding box
        Aabb bounding_box_; //!< Rotated bounding box
        Quaternion<double> rotation_; //!< Rotation transformation

    };

  } // namespace ray
} // namespace cannon

#endif /* ifndef CANNON_RAY_HITTABLE_H */
