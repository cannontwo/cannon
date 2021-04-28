#pragma once
#ifndef CANNON_MATH_RANDOM_DOUBLE_H
#define CANNON_MATH_RANDOM_DOUBLE_H 

#include <random>
#include <cassert>

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace math {

    double random_double();

    double random_double(double min, double max);

    Vector3d random_vec();

    Vector3d random_vec(double min, double max);

    Vector3d random_in_unit_sphere();

    Vector3d random_unit_vec();

    Vector3d random_in_hemisphere(const Vector3d& normal);

  } // namespace math
} // namespace cannon

#endif /* ifndef CANNON_MATH_RANDOM_DOUBLE_H */
