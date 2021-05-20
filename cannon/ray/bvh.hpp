#pragma once
#ifndef CANNON_RAY_BVH_H
#define CANNON_RAY_BVH_H 

/*!
 * \file cannon/ray/bvh.hpp
 * \brief File containing BvhNode class definition.
 */ 

#include <algorithm>

#include <cannon/ray/hittable.hpp>
#include <cannon/ray/hittable_list.hpp>
#include <cannon/math/random_double.hpp>
#include <cannon/log/registry.hpp>
#include <cannon/utils/statistics.hpp>

using namespace cannon::math;
using namespace cannon::log;
using namespace cannon::utils;

// Free Functions

namespace cannon {
  namespace ray {

    /*!
     * Generic comparator around input axis for two Hittables.
     */
    inline bool box_compare(const std::shared_ptr<Hittable> a, const
        std::shared_ptr<Hittable> b, int axis);

    /*!
     * Comparator around x axis for two Hittables.
     */
    bool box_x_compare(const std::shared_ptr<Hittable> a, const
        std::shared_ptr<Hittable> b);

    /*!
     * Comparator around y axis for two Hittables.
     */
    bool box_y_compare(const std::shared_ptr<Hittable> a, const
        std::shared_ptr<Hittable> b);

    /*!
     * Comparator around z axis for two Hittables.
     */
    bool box_z_compare(const std::shared_ptr<Hittable> a, const
        std::shared_ptr<Hittable> b);

    /*!
     * \brief Class representing a node in a bounding volume hierarchy.
     */
    class BvhNode : public Hittable {
      public:

        /*!
         * Default constructor.
         */
        BvhNode() {}

        /*!
         * Constructor taking a HittableList and time interval.
         */
        BvhNode(std::shared_ptr<Affine3d> object_to_world, const
            std::shared_ptr<HittableList> list, double time_0, double time_1) :
          BvhNode(object_to_world, list->objects_, 0, list->objects_.size(),
              time_0, time_1) {}

        /*!
         * Constructor taking a vector of Hittables to include and a time
         * interval. Splits objects along a random axis in order to generate
         * child nodes.
         */
        BvhNode(std::shared_ptr<Affine3d> object_to_world, const
            std::vector<std::shared_ptr<Hittable>>& src_objects, size_t start,
            size_t end, double time_0, double time_1) :
          Hittable(object_to_world) {

          auto objects = src_objects; // Copy in order to modify locally
        
          int axis = static_cast<int>(random_double(0, 3));
          auto comparator = (axis == 0) ? box_x_compare
                            : (axis == 1) ? box_y_compare
                            : box_z_compare;
          
          size_t object_span = end - start;

          if (object_span == 1) {
            left_ = right_ = objects[start]; 
          } else if (object_span == 2) {
            if (comparator(objects[start], objects[start+1])) {
              left_ = objects[start];
              right_ = objects[start+1];
            } else {
              left_ = objects[start+1];
              right_ = objects[start];
            }
          } else {
            std::sort(objects.begin() + start, objects.begin() + end, comparator);

            auto mid = start + object_span / 2;

            auto t = std::make_shared<Affine3d>(Affine3d::Identity());
            left_ = std::make_shared<BvhNode>(t, objects, start, mid, time_0, time_1);
            right_ = std::make_shared<BvhNode>(t, objects, mid, end, time_0, time_1);
          }

          Aabb box_left, box_right;
          if (!left_->bounding_box(time_0, time_1, box_left) ||
              !right_->bounding_box(time_0, time_1, box_right))
            log_error("No bounding box in BvhNode constructor");

          box_ = surrounding_box(box_left, box_right);
        }

        /*
         * Destructor.
         */
        virtual ~BvhNode() {}

        /*!
         * Inherited from Hittable.
         */
        virtual bool object_space_hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;

        /*!
         * Inherited from Hittable.
         */
        virtual bool object_space_bounding_box(double time_0, double time_1, Aabb& output_box) const override;


      public:
        std::shared_ptr<Hittable> left_; //!< Left child
        std::shared_ptr<Hittable> right_; //!< Right child
        Aabb box_; //!< Bounding box

    };

  } // namespace ray
} // namespace cannon


#endif /* ifndef CANNON_RAY_BVH_H */
