#include <cannon/ray/moving_sphere.hpp>

using namespace cannon::ray;

bool MovingSphere::object_space_hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
  Vector3d oc = r.orig_ - center(r.time_);
  auto a = r.dir_.dot(r.dir_);
  auto b = oc.dot(r.dir_);
  auto c = oc.dot(oc) - radius_ * radius_;

  auto discriminant = b*b - a*c;
  if (discriminant < 0)
    return false;

  auto sqrt_d = std::sqrt(discriminant);

  // Find nearest root of the quadratic that lies between t_min and t_max
  auto root = (-b - sqrt_d) / a;
  if (root < t_min || t_max < root) {
    root = (-b + sqrt_d) / a;
    if (root < t_min || t_max < root)
      return false;
  }

  rec.t = root;
  rec.p = r.at(rec.t);
  Vector3d outward_normal = (rec.p - center(r.time_)) / radius_;
  rec.set_face_normal(r, outward_normal);
  rec.mat_ptr = mat_ptr_;
  
  return true;
}

Vector3d MovingSphere::center(double time) const {
  return center_0_ + ((time - time_0_) / (time_1_ - time_0_)) * (center_1_ - center_0_);
}

bool MovingSphere::object_space_bounding_box(double time_0, double time_1, Aabb& output_box) const {
  Aabb box_0(center(time_0) - Vector3d(radius_, radius_, radius_),
      center(time_0) + Vector3d(radius_, radius_, radius_));

  Aabb box_1(center(time_1) - Vector3d(radius_, radius_, radius_),
      center(time_1) + Vector3d(radius_, radius_, radius_));

  output_box = surrounding_box(box_0, box_1);
  return true;
}
