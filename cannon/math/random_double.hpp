#pragma once
#ifndef CANNON_MATH_RANDOM_DOUBLE_H
#define CANNON_MATH_RANDOM_DOUBLE_H 

/*!
 * \file cannon/math/random_double.hpp
 * \brief File containing utilities for generating random doubles and vectors.
 */

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace math {

    /*!
     * \brief Generate a random double uniformly at random between 0 and 1 in a
     * thread-safe way.
     *
     * \returns The generated double.
     */
    double random_double();

    /*!
     * \brief Generate a random double between min and max.
     *
     * \param min The minimum number that can be generated.
     * \param max The maximum number that can be generated.
     *
     * \returns The generated double.
     */
    double random_double(double min, double max);

    /*!
     * \brief Generate a random vector with entries sampled uniformly at random
     * between 0 and 1.
     *
     * \returns The generated vector.
     */
    Vector3d random_vec();

    /*!
     * \brief Generate a random vector with entries sampled uniformly at random
     * between the input minimum and maximum.
     *
     * \param min The minimum value to generate.
     * \param max The maximum value to generate.
     *
     * \returns The generated vector.
     */
    Vector3d random_vec(double min, double max);

    /*!
     * \brief Generate a random vector in the unit sphere.
     *
     * \returns The generated vector.
     */
    Vector3d random_in_unit_sphere();

    /*!
     * \brief Generate a random unit vector.
     * 
     * \returns The generated vector.
     */
    Vector3d random_unit_vec();

    /*!
     * \brief Generate a random vector in the unit hemisphere around the input
     * normal vector.
     *
     * \param normal The normal vector to sample around.
     *
     * \returns The generated vector.
     */
    Vector3d random_in_hemisphere(const Vector3d& normal);

    /*!
     * \brief Generate a random vector in the unit disk in the X-Y plane.
     *
     * \returns The generated vector.
     */
    Vector3d random_in_disk();

  } // namespace math
} // namespace cannon

#endif /* ifndef CANNON_MATH_RANDOM_DOUBLE_H */
