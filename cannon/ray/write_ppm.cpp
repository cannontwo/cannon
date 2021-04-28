#include <cannon/ray/write_ppm.hpp>

using namespace cannon::ray;

void cannon::ray::write_color(std::ostream& os, const Vector3d& pixel_color) {
  os << static_cast<int>(255.999 * pixel_color[0]) << " "
     << static_cast<int>(255.999 * pixel_color[1]) << " "
     << static_cast<int>(255.999 * pixel_color[2]) << "\n";
}

void cannon::ray::write_color(std::ostream& os, const Vector3d& pixel_color,
    unsigned int samples_per_pixel) {
  auto r = pixel_color.x();
  auto g = pixel_color.y();
  auto b = pixel_color.z();

  // Divide color by number of samples and correct for gamma=2.0
  auto scale = 1.0 / samples_per_pixel;
  r = std::sqrt(scale * r);
  g = std::sqrt(scale * g);
  b = std::sqrt(scale * b);

  // write value of each color component
  os << static_cast<int>(256 * std::min(0.999, std::max(0.0, r))) << " "
     << static_cast<int>(256 * std::min(0.999, std::max(0.0, g))) << " "
     << static_cast<int>(256 * std::min(0.999, std::max(0.0, b))) << "\n";
}
