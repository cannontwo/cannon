#include <cannon/ray/camera.hpp>

#include <cannon/ray/ray.hpp>

using namespace cannon::ray;

Ray Camera::get_ray(double s, double t) const {
  Vector3d rd = lens_radius_ * random_in_disk();
  Vector3d offset = u_ * rd.x() + v_ * rd.y();

  return Ray(origin_ + offset, 
      lower_left_corner_ + s*horizontal_ + t*vertical_ - origin_ - offset, 
      random_double(time_0_, time_1_));
}

Ray Camera::get_ray(double s, double t, const CameraSample& sample) const {
  Vector3d lens_sample = Vector3d::Zero();

  // Transform p_lens from [0, 1]^2 to [-1, 1]^2
  lens_sample.x() = sample.p_lens.x() * 2 - 1.0;
  lens_sample.y() = sample.p_lens.y() * 2 - 1.0;

  // Apply concentric mapping to map to disk
  double theta, r;
  if (std::abs(lens_sample.x()) > std::abs(lens_sample.y())) {
    r = lens_sample.x();
    theta = (M_PI / 4.0) * (lens_sample.y() / lens_sample.x());
  } else {
    r = lens_sample.y();
    theta = (M_PI / 2.0) - ((M_PI / 4.0) * (lens_sample.x() / lens_sample.y()));
  }

  lens_sample.head(2) = r * Vector2d(std::cos(theta), std::sin(theta));

  Vector3d rd = lens_radius_ * lens_sample;
  Vector3d offset = u_ * rd.x() + v_ * rd.y();

  return Ray(origin_ + offset, 
      lower_left_corner_ + s*horizontal_ + t*vertical_ - origin_ - offset, 
      time_0_ + (time_1_ - time_0_) * sample.time);
}
