#pragma once
#ifndef CANNON_MOVING_SPHERE_H
#define CANNON_MOVING_SPHERE_H 

/*!
 * \file cannon/ray/moving_sphere.hpp
 * File containing MovingSphere class definition
 */

#include <cannon/ray/hittable.hpp>

namespace cannon {
  namespace ray {

    /*!
     * \brief Class representing a sphere that moves over time.
     */
    class MovingSphere : public Hittable {
      public:

        /*!
         * Default constructor.
         */
        MovingSphere() {}

        /*!
         * Constructor taking sphere centers at start and end time, radius, and material.
         */
        MovingSphere(const Vector3d& center_0, const Vector3d& center_1, double
            time_0, double time_1, double r, std::shared_ptr<Material> m) :
          center_0_(center_0), center_1_(center_1), time_0_(time_0),
          time_1_(time_1), radius_(r), mat_ptr_(m) {}

        /*!
         * Destructor.
         */
        virtual ~MovingSphere() {}

        /*!
         * Inherited from Hittable.
         */
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;

        /*!
         * Inherited from Hittable.
         */
        virtual bool bounding_box(double time_0, double time_1, Aabb& output_box) const override;

        /*!
         * Method to compute sphere center location at input time.
         *
         * \param time Time at which to compute center
         *
         * \returns Center location
         */
        Vector3d center(double time) const;

      public:
        Vector3d center_0_, center_1_;
        double time_0_, time_1_;
        double radius_;
        std::shared_ptr<Material> mat_ptr_;

    };

  } // namespace ray
} // namespace cannon

#endif /* ifndef CANNON_MOVING_SPHERE_H */
