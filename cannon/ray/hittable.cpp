#include <cannon/ray/hittable.hpp>

using namespace cannon::ray;


bool Translate::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
  Ray moved_r(r.orig_ - offset_, r.dir_, r.time_);

  if (!ptr_->hit(moved_r, t_min, t_max, rec))
    return false;

  rec.p += offset_;
  rec.set_face_normal(moved_r, rec.normal);

  return true;
}

bool Translate::bounding_box(double time_0, double time_1, Aabb& output_box) const {
  if (!ptr_->bounding_box(time_0, time_1, output_box))
    return false;

  output_box = Aabb(output_box.minimum_ + offset_, output_box.maximum_ + offset_);
  return true;
}

bool Rotate::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
  Vector3d rot_origin = rotation_.inverse() * r.orig_;
  Vector3d rot_dir = rotation_.inverse() * r.dir_;

  Ray rotated_r(rot_origin, rot_dir, r.time_);

  if (!ptr_->hit(rotated_r, t_min, t_max, rec))
    return false;

  Vector3d rot_p = rotation_ * rec.p;
  Vector3d rot_normal = rotation_ * rec.normal;

  rec.p = rot_p;
  rec.set_face_normal(rotated_r, rot_normal);

  return true;
}
