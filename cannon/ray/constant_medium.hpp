#pragma once
#ifndef CANNON_RAY_CONSTANT_MEDIUM_H
#define CANNON_RAY_CONSTANT_MEDIUM_H 

/*!
 * \file cannon/ray/constant_medium.hpp
 * File containing ConstantMedium class definition.
 */

#include <cannon/ray/hittable.hpp>
#include <cannon/ray/texture.hpp>
#include <cannon/ray/material.hpp>

namespace cannon {
  namespace ray {

    class ConstantMedium : public Hittable {
      public:
        ConstantMedium(std::shared_ptr<Hittable> b, double d,
            std::shared_ptr<Texture> a) : boundary_(b), neg_inv_density_(-1/d),
        phase_function_(std::make_shared<Isotropic>(a)) {}

        ConstantMedium(std::shared_ptr<Hittable> b, double d, const Vector3d&
            c) : boundary_(b), neg_inv_density_(-1/d),
        phase_function_(std::make_shared<Isotropic>(c)) {}

        /*!
         * Destructor.
         */
        virtual ~ConstantMedium() {}

        /*!
         * Inherited from Hittable.
         */
        virtual bool object_space_hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;

        /*!
         * Inherited from Hittable.
         */
        virtual bool object_space_bounding_box(double time_0, double time_1, Aabb& output_box) const override {
          return boundary_->bounding_box(time_0, time_1, output_box);
        }

      public:
        std::shared_ptr<Hittable> boundary_; //!< Scattering medium geometry
        double neg_inv_density_; //!< 1 / Optical density of medium
        std::shared_ptr<Material> phase_function_; //!< Material determining color
    };

  } // namespace ray
} // namespace cannon

#endif /* ifndef CANNON_RAY_CONSTANT_MEDIUM_H */
