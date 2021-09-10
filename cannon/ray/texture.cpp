#include <cannon/ray/texture.hpp>

using namespace cannon::ray;

Vector3d CheckerTexture::value(double u, double v, const Vector3d& p) const {
  auto sines = sin(10*p.x()) * sin(10*p.y()) * sin(10 * p.z());

  if (sines < 0)
    return odd_->value(u, v, p);
  else
    return even_->value(u, v, p);
}

Vector3d ImageTexture::value(double u, double v, const Vector3d& /*p*/) const {
  // Return cyan if no data
  if (data_ == nullptr)
    return Vector3d(0, 1, 1);

  u = std::min(1.0, std::max(0.0, u));
  v = 1.0 - std::min(1.0, std::max(0.0, v)); // Flip v to image coords

  auto i = static_cast<int>(u * width_);
  auto j = static_cast<int>(v * height_);

  // Clamp integers
  if (i >= width_)
    i = width_ - 1;

  if (j >= height_)
    j = height_ - 1;

  auto color_scale = 1.0 / 255.0;
  auto pixel = data_ + j*bytes_per_scanline_ + i*bytes_per_pixel;

  return Vector3d(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
}
