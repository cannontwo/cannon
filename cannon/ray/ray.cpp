#include <cannon/ray/ray.hpp>

using namespace cannon::ray;

Vector3d Ray::at(double t) const {
  return orig_ + t * dir_;
}
