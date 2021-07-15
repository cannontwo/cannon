#include <cannon/ray/write_ppm.hpp>

#include <cassert>

#include <cannon/ray/film.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::ray;
using namespace cannon::log;

void cannon::ray::write_color(std::ostream& os, const Vector3d& pixel_color) {
  os << static_cast<int>(255.999 * pixel_color[0]) << " "
     << static_cast<int>(255.999 * pixel_color[1]) << " "
     << static_cast<int>(255.999 * pixel_color[2]) << "\n";
}

void cannon::ray::write_color(std::ostream& os, const Vector3d& pixel_color,
    double filter_sum) {
  assert(filter_sum != 0.0);

  auto r = pixel_color.x();
  auto g = pixel_color.y();
  auto b = pixel_color.z();

  // Divide color by number of samples and correct for gamma=2.0
  auto scale = 1.0 / filter_sum;
  r = std::sqrt(scale * r);
  g = std::sqrt(scale * g);
  b = std::sqrt(scale * b);

  // write value of each color component
  os << static_cast<int>(256 * std::min(0.999, std::max(0.0, r))) << " "
     << static_cast<int>(256 * std::min(0.999, std::max(0.0, g))) << " "
     << static_cast<int>(256 * std::min(0.999, std::max(0.0, b))) << "\n";
}

void cannon::ray::write_colors(std::ostream &os,
                               const Ref<const MatrixXd> &pixel_colors,
                               unsigned int samples_per_pixel) {
  if (pixel_colors.rows() != 3) {
    throw std::runtime_error("Input color matrix had incorrect number of rows");
  }

  for (unsigned int i = 0; i < pixel_colors.cols(); i++) {
    write_color(os, pixel_colors.col(i), samples_per_pixel);
  }
}

void cannon::ray::write_colors(std::ostream& os, const std::vector<FilmPixel>& pixels) {
  for (unsigned int i = 0; i < pixels.size(); i++) {
    write_color(os, pixels[i].color_sum_, pixels[i].filter_weight_sum_);
  }
}
