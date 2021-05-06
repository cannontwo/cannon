#pragma once
#ifndef CANNON_RAY_AA_RECT_H
#define CANNON_RAY_AA_RECT_H 

/*!
 * \file cannon/ray/aa_rect.hpp
 * File containing axis-aligned rectangle class definitions.
 */

#include <cannon/ray/hittable.hpp>
#include <cannon/ray/hittable_list.hpp>

namespace cannon {
  namespace ray {

    /*!
     * \brief Class representing a Z-axis aligned rectangle.
     */
    class XYRect : public Hittable {
      public:

        /*!
         * Default constructor.
         */
        XYRect() {}

        /*!
         * Constructor taking dimensions of rectangle geometry, depth along z
         * axis, and material for this rectangle.
         */
        XYRect(double x0, double x1, double y0, double y1, double k,
            std::shared_ptr<Material> mat) : x0_(x0), x1_(x1), y0_(y0),
        y1_(y1), k_(k), mat_ptr_(mat) {}

        /*!
         * Destructor.
         */
        virtual ~XYRect() {}

        /*!
         * Inherited from Hittable.
         */
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;

        /*!
         * Inherited from Hittable.
         */
        virtual bool bounding_box(double time_0, double time_1, Aabb& output_box) const override {
          // Bounding box must be nonzero in each dimension
          output_box = Aabb(Vector3d(x0_, y0_, k_ - 1e-3), Vector3d(x1_, y1_, k_ + 1e-3));
          return true;
        }

      public:
        double x0_, x1_, y0_, y1_, k_; //!< Rectangle extent parameters
        std::shared_ptr<Material> mat_ptr_; //!< Material
    };

    /*!
     * \brief Class representing a Y-axis aligned rectangle.
     */
    class XZRect : public Hittable {
      public:

        /*!
         * Default constructor.
         */
        XZRect() {}

        /*!
         * Constructor taking dimensions of rectangle geometry, depth along z
         * axis, and material for this rectangle.
         */
        XZRect(double x0, double x1, double z0, double z1, double k,
            std::shared_ptr<Material> mat) : x0_(x0), x1_(x1), z0_(z0),
        z1_(z1), k_(k), mat_ptr_(mat) {}

        /*!
         * Destructor.
         */
        virtual ~XZRect() {}

        /*!
         * Inherited from Hittable.
         */
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;

        /*!
         * Inherited from Hittable.
         */
        virtual bool bounding_box(double time_0, double time_1, Aabb& output_box) const override {
          // Bounding box must be nonzero in each dimension
          output_box = Aabb(Vector3d(x0_, k_ - 1e-3, z0_), Vector3d(x1_, k_ + 1e-3, z1_));
          return true;
        }

      public:
        double x0_, x1_, z0_, z1_, k_; //!< Rectangle extent parameters
        std::shared_ptr<Material> mat_ptr_; //!< Material
    };

    /*!
     * \brief Class representing a X-axis aligned rectangle.
     */
    class YZRect : public Hittable {
      public:

        /*!
         * Default constructor.
         */
        YZRect() {}

        /*!
         * Constructor taking dimensions of rectangle geometry, depth along z
         * axis, and material for this rectangle.
         */
        YZRect(double y0, double y1, double z0, double z1, double k,
            std::shared_ptr<Material> mat) : y0_(y0), y1_(y1), z0_(z0),
        z1_(z1), k_(k), mat_ptr_(mat) {}

        /*!
         * Destructor.
         */
        virtual ~YZRect() {}

        /*!
         * Inherited from Hittable.
         */
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;

        /*!
         * Inherited from Hittable.
         */
        virtual bool bounding_box(double time_0, double time_1, Aabb& output_box) const override {
          // Bounding box must be nonzero in each dimension
          output_box = Aabb(Vector3d(k_ - 1e-3, y0_, z0_), Vector3d(k_ + 1e-3, y1_, z1_));
          return true;
        }

      public:
        double y0_, y1_, z0_, z1_, k_; //!< Rectangle extent parameters
        std::shared_ptr<Material> mat_ptr_; //!< Material
    };

    /*!
     * \brief Class representing a box with axis-aligned sides.
     */
    class Box : public Hittable {
      public:

        /*!
         * Default constructor.
         */
        Box() {}

        /*!
         * Constructor taking minimum and maximum corner of the box, as well as
         * material for its sides.
         */
        Box(const Vector3d& p0, const Vector3d& p1, std::shared_ptr<Material>
            mat_ptr) : box_min_(p0), box_max_(p1) {

          sides_.add(std::make_shared<XYRect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), mat_ptr));
          sides_.add(std::make_shared<XYRect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), mat_ptr));

          sides_.add(std::make_shared<XZRect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), mat_ptr));
          sides_.add(std::make_shared<XZRect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), mat_ptr));

          sides_.add(std::make_shared<YZRect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), mat_ptr));
          sides_.add(std::make_shared<YZRect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), mat_ptr));
        }

        /*!
         * Destructor.
         */
        virtual ~Box() {}

        /*!
         * Inherited from Hittable.
         */
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override {
          return sides_.hit(r, t_min, t_max, rec);
        }

        /*!
         * Inherited from Hittable.
         */
        virtual bool bounding_box(double time_0, double time_1, Aabb& output_box) const override {
          output_box = Aabb(box_min_, box_max_);
          return true;
        }

      public:
        Vector3d box_min_;
        Vector3d box_max_;
        HittableList sides_;

    };


  } // namespace ray
} // namespace cannon

#endif /* ifndef CANNON_RAY_AA_RECT_H */
