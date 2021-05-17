#include <cmath>
#include <random>

#include <cannon/graphics/random_color.hpp>

using namespace cannon::graphics;

Vector4f cannon::graphics::generate_random_color(float saturation, float value) {
  Vector4f ret_vec = Vector4f::Ones();

  static std::random_device rd;
  static std::default_random_engine gen(rd());
  static std::uniform_real_distribution<float> d(0.0, 360.0);
  float hue = d(gen);

  auto rgb_part = hsv_to_rgb(hue, saturation, value);
  ret_vec.head(3) = rgb_part;

  return ret_vec;
}

Vector3f cannon::graphics::hsv_to_rgb(float hue, float saturation, float value) {
  assert(0.0 <= hue);
  assert(hue <= 360.0);
  assert(0 <= saturation);
  assert(saturation <= 1);
  assert(0 <= value);
  assert(value <= 1);

  // Adapted from https://www.rapidtables.com/convert/color/hsv-to-rgb.html
  float c = value * saturation;
  float x = c * (1.0 - std::fabs(std::fmod((hue / 60), 2) - 1));
  float m = value - c;

  Vector3f ret_vec = Vector3f::Zero();

  if (hue <= 60) {
    ret_vec[0] = c;
    ret_vec[1] = x;
  } else if (hue <= 120) {
    ret_vec[0] = x;
    ret_vec[1] = c;
  } else if (hue <= 180) {
    ret_vec[1] = c;
    ret_vec[2] = x;
  } else if (hue <= 240) {
    ret_vec[1] = x;
    ret_vec[2] = c;
  } else if (hue <= 300) {
    ret_vec[0] = x;
    ret_vec[2] = c;
  } else { 
    ret_vec[0] = c;
    ret_vec[2] = x;
  }

  ret_vec += Vector3f::Ones() * m;
  return ret_vec;
}
