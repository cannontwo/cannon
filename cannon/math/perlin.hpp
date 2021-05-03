#pragma once
#ifndef CANNON_MATH_PERLIN_H
#define CANNON_MATH_PERLIN_H 

/*!
 * \file cannon/math/perlin.hpp
 * File containing Perlin class definition.
 */

#include <cannon/math/random_double.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::log;

namespace cannon {
  namespace math {

    // Free Functions

    /*!
     * Generate permutation of noise.
     *
     * \returns Permutation of length Perlin::point_count_;
     */
    int* perlin_generate_perm();

    /*!
     * Permute the input int array of length n.
     * 
     * \param p The array to permute.
     * \param n The length of the input array.
     */
    void permute(int* p, int n);

    /*!
     * \brief Class representing a perlin noise generator.
     */
    class Perlin {
      public:

        /*!
         * Default constructor.
         */
        Perlin() {
          ranfloat_ = new double[point_count_];
          for (int i = 0; i < point_count_; i++) {
            ranfloat_[i] = random_double();
          }

          perm_x_ = perlin_generate_perm();
          perm_y_ = perlin_generate_perm();
          perm_z_ = perlin_generate_perm();
        }

        /*!
         * Destructor.
         */
        ~Perlin() {
          delete[] ranfloat_;
          delete[] perm_x_;
          delete[] perm_y_;
          delete[] perm_z_;
        }

        /*!
         * Get Perlin noise value for an input 3D point.
         *
         * \param p The point to evaluate.
         *
         * \returns Noise value.
         */
        double noise(const Vector3d& p) const;

        static const int point_count_ = 256; //!< Number of random floats to generate.

      private:
        double* ranfloat_; //!< Array of cached random floats.
        int* perm_x_; //!< X-axis permutation.
        int* perm_y_; //!< Y-axis permutation.
        int* perm_z_; //!< Z-axis permutation.
    };


  } // namespace math
} // namespace cannon

#endif /* ifndef CANNON_MATH_PERLIN_H */
