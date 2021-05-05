#include <cannon/ray/aa_rect.hpp>

using namespace cannon::ray;

bool XYRect::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
  auto t = (k_ - r.orig_.z()) / r.dir_.z();

  if (t < t_min || t > t_max)
    return false;

  auto x = r.orig_.x() + t * r.dir_.x();
  auto y = r.orig_.y() + t * r.dir_.y();
  if (x < x0_ || x > x1_ || y < y0_ || y > y1_)
    return false;

  // Fill out hit pointer
  rec.u = (x - x0_) / (x1_ - x0_);
  rec.v = (y - y0_) / (y1_ - y0_);
  rec.t = t;
  rec.set_face_normal(r, Vector3d(0, 0, 1));
  rec.mat_ptr = mat_ptr_;
  rec.p = r.at(t);
  return true;
}

bool XZRect::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
  auto t = (k_ - r.orig_.y()) / r.dir_.y();

  if (t < t_min || t > t_max)
    return false;

  auto x = r.orig_.x() + t * r.dir_.x();
  auto z = r.orig_.z() + t * r.dir_.z();
  if (x < x0_ || x > x1_ || z < z0_ || z > z1_)
    return false;

  // Fill out hit pointer
  rec.u = (x - x0_) / (x1_ - x0_);
  rec.v = (z - z0_) / (z1_ - z0_);
  rec.t = t;
  rec.set_face_normal(r, Vector3d(0, 1, 0));
  rec.mat_ptr = mat_ptr_;
  rec.p = r.at(t);
  return true;
}

bool YZRect::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
  auto t = (k_ - r.orig_.x()) / r.dir_.x();

  if (t < t_min || t > t_max)
    return false;

  auto y = r.orig_.y() + t * r.dir_.y();
  auto z = r.orig_.z() + t * r.dir_.z();
  if (y < y0_ || y > y1_ || z < z0_ || z > z1_)
    return false;

  // Fill out hit pointer
  rec.u = (y - y0_) / (y1_ - y0_);
  rec.v = (z - z0_) / (z1_ - z0_);
  rec.t = t;
  rec.set_face_normal(r, Vector3d(1, 0, 0));
  rec.mat_ptr = mat_ptr_;
  rec.p = r.at(t);
  return true;
}
