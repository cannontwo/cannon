#include <cannon/ray/constant_medium.hpp>

using namespace cannon::ray;

bool ConstantMedium::object_space_hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
  hit_record rec1, rec2;

  // Initial hit
  if (!boundary_->hit(r, -std::numeric_limits<double>::infinity(),
        std::numeric_limits<double>::infinity(), rec1))
    return false;

  // Hit another side of boundary (Note this assumes convex geometry)
  if (!boundary_->hit(r, rec1.t + 1e-3,
        std::numeric_limits<double>::infinity(), rec2))
    return false;

  if (rec1.t < t_min)
    rec1.t = t_min;

  if (rec2.t > t_max)
    rec2.t = t_max;

  if (rec1.t >= rec2.t)
    return false;

  if (rec1.t < 0)
    rec1.t = 0;

  const auto ray_length = r.dir_.norm();
  const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
  
  // Random scattering distance
  const auto hit_distance = neg_inv_density_ * std::log(random_double());

  if (hit_distance > distance_inside_boundary)
    return false;

  rec.t = rec1.t + hit_distance / ray_length;
  rec.p = r.at(rec.t);

  rec.normal = Vector3d(1, 0, 0); // Arbitrary
  rec.front_face = true;          // Arbitrary
  rec.mat_ptr = phase_function_;

  return true;
}
