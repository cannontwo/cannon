#ifndef CANNON_GRAPHICS_RANDOM_COLOR_H
#define CANNON_GRAPHICS_RANDOM_COLOR_H 

#include <cmath>
#include <random>

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace graphics {

    Vector4f generate_random_color(float saturation=0.75f, float value=.90f);

    Vector3f hsv_to_rgb(float hue, float saturation, float value);

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_RANDOM_COLOR_H */
