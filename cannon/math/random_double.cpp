#include <cannon/math/random_double.hpp>

using namespace cannon::math;

double cannon::math::random_double() {
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  static std::mt19937 generator;

  return distribution(generator);
}

double cannon::math::random_double(double min, double max) {
  return min + (max - min)*random_double();
}

Vector3d cannon::math::random_vec() {
  Vector3d ret_vec;
  ret_vec << random_double(),
             random_double(),
             random_double();

  return ret_vec;
}

Vector3d cannon::math::random_vec(double min, double max) {
  Vector3d ret_vec;
  ret_vec << random_double(min, max),
             random_double(min, max),
             random_double(min, max);

  return ret_vec;
}

Vector3d cannon::math::random_in_unit_sphere() {
  // Note this is rejection sampling, and there's definitely a better way.
  while (true) {
    auto p = random_vec(-1, 1);
    if (p.dot(p) >= 1)
      continue;
    else
      return p;
  }
}

Vector3d cannon::math::random_unit_vec() {
  return random_in_unit_sphere().normalized();
}

Vector3d cannon::math::random_in_hemisphere(const Vector3d& normal) {
  Vector3d random_vec = random_in_unit_sphere();

  if (random_vec.dot(normal) > 0.0)
    return random_vec;
  else
    return -random_vec;
}
