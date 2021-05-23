#include <cannon/ray/sphere.hpp>

#include <cannon/ray/aabb.hpp>

using namespace cannon::ray;

bool Sphere::object_space_hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
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
  get_sphere_uv(outward_normal, rec.u, rec.v);
  rec.mat_ptr = mat_ptr_;
  
  return true;
}

bool Sphere::object_space_bounding_box(double time_0, double time_1, Aabb& output_box) const {
  output_box = Aabb(center_ - Vector3d(radius_, radius_, radius_), center_ +
      Vector3d(radius_, radius_, radius_));
  return true;
}

// Free Functions
void cannon::ray::get_sphere_uv(const Vector3d& p, double& u, double& v) {
  auto theta = std::acos(-p.y());
  auto phi = std::atan2(-p.z(), p.x()) + M_PI;

  u = phi / (2 * M_PI);
  v = theta / M_PI;
}
