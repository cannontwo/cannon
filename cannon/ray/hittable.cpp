#include <cannon/ray/hittable.hpp>

#include <cannon/ray/ray.hpp>
#include <cannon/ray/aabb.hpp>

using namespace cannon::ray;

void hit_record::set_face_normal(const Ray& r, const Vector3d& outward_normal) {
  front_face = r.dir_.dot(outward_normal) < 0;
  normal = front_face ? outward_normal : -outward_normal;
}

bool Hittable::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const { 
  Vector3d object_space_origin = (*world_to_object_) * r.orig_;
  Vector3d object_space_dir = world_to_object_->linear() * r.dir_;

  Ray object_space_ray(object_space_origin, object_space_dir, r.time_);

  if (!object_space_hit(object_space_ray, t_min, t_max, rec))
    return false;

  Vector3d world_space_p = (*object_to_world_) * rec.p;
  Vector3d world_space_normal = object_to_world_->linear() * rec.normal;

  rec.p = world_space_p;
  rec.set_face_normal(object_space_ray, world_space_normal);

  return true;
}

bool Hittable::bounding_box(double time_0, double time_1, Aabb& output_box) const {
  Aabb object_space_box;
  bool has_box = object_space_bounding_box(time_0, time_1, object_space_box);

  if (!has_box)
    return false;

  Vector3d min(std::numeric_limits<double>::infinity(),
               std::numeric_limits<double>::infinity(),
               std::numeric_limits<double>::infinity());

  Vector3d max(-std::numeric_limits<double>::infinity(),
               -std::numeric_limits<double>::infinity(),
               -std::numeric_limits<double>::infinity());

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {

        Vector3d corner(i*object_space_box.maximum_.x() + (1-i)*object_space_box.minimum_.x(),
                        j*object_space_box.maximum_.y() + (1-j)*object_space_box.minimum_.y(),
                        k*object_space_box.maximum_.z() + (1-k)*object_space_box.minimum_.z());

        Vector3d trans_corner = (*object_to_world_) * corner;

        for (int c = 0; c < 3; c++) {
          min[c] = std::fmin(min[c], trans_corner[c]);
          max[c] = std::fmax(max[c], trans_corner[c]);
        }

      }
    }
  }

  output_box = Aabb(min, max);

  return true;
}
