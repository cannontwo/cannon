#include <cannon/ray/camera.hpp>

using namespace cannon::ray;

Ray Camera::get_ray(double u, double v) const {
  return Ray(origin_, lower_left_corner_ + u*horizontal_ + v*vertical_ - origin_);
}
