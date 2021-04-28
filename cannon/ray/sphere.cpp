#include <cannon/ray/sphere.hpp>

using namespace cannon::ray;

bool Sphere::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
  Vector3d oc = r.orig_ - center_;
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
  Vector3d outward_normal = (rec.p - center_) / radius_;
  rec.set_face_normal(r, outward_normal);
  rec.mat_ptr = mat_ptr_;
  
  return true;
}
