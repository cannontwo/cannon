#include <cannon/ray/aabb.hpp>

using namespace cannon::ray;

bool Aabb::hit(const Ray& r, double t_min, double t_max) const {
  for (int i = 0; i < 3; i++) {
    auto inv_d = 1.0f / r.dir_[i];
    auto t0 = (minimum_[i] - r.orig_[i]) * inv_d;
    auto t1 = (maximum_[i] - r.orig_[i]) * inv_d;

    if (inv_d < 0.0)
      std::swap(t0, t1);

    t_min = t0 > t_min ? t0 : t_min;
    t_max = t1 < t_max ? t1 : t_max;

    if (t_max <= t_min)
      return false;
  }

  return true;
}

// Free functions
Aabb cannon::ray::surrounding_box(const Aabb& box_0, const Aabb& box_1) {
  Vector3d small(std::fmin(box_0.minimum_.x(), box_1.minimum_.x()),
      std::fmin(box_0.minimum_.y(), box_1.minimum_.y()), 
      std::fmin(box_0.minimum_.z(), box_1.minimum_.z()));

  Vector3d big(std::fmax(box_0.maximum_.x(), box_1.maximum_.x()),
      std::fmax(box_0.maximum_.y(), box_1.maximum_.y()), 
      std::fmax(box_0.maximum_.z(), box_1.maximum_.z()));

  return Aabb(small, big);
}

Aabb cannon::ray::surrounding_box(const Aabb& box_0, const Vector3d& p) {
  Vector3d small(std::fmin(box_0.minimum_.x(), p.x()),
      std::fmin(box_0.minimum_.y(), p.y()), 
      std::fmin(box_0.minimum_.z(), p.z()));

  Vector3d big(std::fmax(box_0.maximum_.x(), p.x()),
      std::fmax(box_0.maximum_.y(), p.y()), 
      std::fmax(box_0.maximum_.z(), p.z()));

  return Aabb(small, big);
}
