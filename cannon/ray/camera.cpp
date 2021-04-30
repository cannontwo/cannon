#include <cannon/ray/camera.hpp>

using namespace cannon::ray;

Ray Camera::get_ray(double s, double t) const {
  Vector3d rd = lens_radius_ * random_in_disk();
  Vector3d offset = u_ * rd.x() + v_ * rd.y();

  return Ray(origin_ + offset, 
      lower_left_corner_ + s*horizontal_ + t*vertical_ - origin_ - offset, 
      random_double(time_0_, time_1_));
}
