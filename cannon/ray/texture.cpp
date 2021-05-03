#include <cannon/ray/texture.hpp>

using namespace cannon::ray;

Vector3d CheckerTexture::value(double u, double v, const Vector3d& p) const {
  auto sines = sin(10*p.x()) * sin(10*p.y()) * sin(10 * p.z());

  if (sines < 0)
    return odd_->value(u, v, p);
  else
    return even_->value(u, v, p);
}
