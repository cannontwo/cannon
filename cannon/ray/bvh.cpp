#include <cannon/ray/bvh.hpp>

using namespace cannon::ray;

bool BvhNode::object_space_hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
  if (!box_.hit(r, t_min, t_max))
    return false;

  bool hit_left = left_->hit(r, t_min, t_max, rec);
  bool hit_right = right_->hit(r, t_min, hit_left ? rec.t : t_max, rec);

  return hit_left || hit_right;
}

bool BvhNode::object_space_bounding_box(double time_0, double time_1, Aabb& output_box) const {
  output_box = box_;
  return true;
}

// Free Functions
inline bool cannon::ray::box_compare(const std::shared_ptr<Hittable> a, const
    std::shared_ptr<Hittable> b, int axis) {
  Aabb box_a;
  Aabb box_b;

  if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
    log_error("No bounding box in BvhNode constructor");

  return box_a.minimum_[axis] < box_b.minimum_[axis];
}

/*!
 * Comparator around x axis for two Hittables.
 */
bool cannon::ray::box_x_compare(const std::shared_ptr<Hittable> a, const
    std::shared_ptr<Hittable> b) {
  return box_compare(a, b, 0);
}

/*!
 * Comparator around y axis for two Hittables.
 */
bool cannon::ray::box_y_compare(const std::shared_ptr<Hittable> a, const
    std::shared_ptr<Hittable> b) {
  return box_compare(a, b, 1);
}

/*!
 * Comparator around z axis for two Hittables.
 */
bool cannon::ray::box_z_compare(const std::shared_ptr<Hittable> a, const
    std::shared_ptr<Hittable> b) {
  return box_compare(a, b, 2);
}
