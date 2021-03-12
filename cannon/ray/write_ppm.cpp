#include <cannon/ray/write_ppm.hpp>

using namespace cannon::ray;

void cannon::ray::write_color(std::ostream& os, const Vector3d& pixel_color) {
  os << static_cast<int>(255.999 * pixel_color[0]) << " "
     << static_cast<int>(255.999 * pixel_color[1]) << " "
     << static_cast<int>(255.999 * pixel_color[2]) << "\n";
}
