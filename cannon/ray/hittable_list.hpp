#pragma once
#ifndef CANNON_RAY_HITTABLE_LIST_H
#define CANNON_RAY_HITTABLE_LIST_H 

/*!
 * \file cannon/ray/hittable_list.hpp
 * File containing HittableList class definition.
 */

#include <memory>

#include <cannon/ray/hittable.hpp>

namespace cannon {
  namespace ray {

    /*!
     * \brief Class representing a list of Hittables, which is itself Hittable.
     */
    class HittableList : public Hittable {
      public:

        /*!
         * Default constructor.
         */
        HittableList() {}

        /*!
         * Constructor taking an initial Hittable to insert.
         */
        HittableList(std::shared_ptr<Hittable> obj) { add(obj); }

        /*!
         * Destructor.
         */
        virtual ~HittableList() {}

        /*!
         * Method to empty this list.
         */
        void clear();

        /*!
         * Method to add a Hittable to this list.
         *
         * \param obj The Hittable to add.
         */
        void add(std::shared_ptr<Hittable> obj);

        /*!
         * Inherited from Hittable.
         */
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;

        /*!
         * Inherited from Hittable.
         */
        virtual bool bounding_box(double time_0, double time_1, Aabb& output_box) const override;

      public:
        std::vector<std::shared_ptr<Hittable>> objects_; //!< Internal vector of Hittables.

    };

  } // namespace ray
} // namespace cannon

#endif /* ifndef CANNON_RAY_HITTABLE_LIST_H */
