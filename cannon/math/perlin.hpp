#pragma once
#ifndef CANNON_MATH_PERLIN_H
#define CANNON_MATH_PERLIN_H 

/*!
 * \file cannon/math/perlin.hpp
 * File containing Perlin class definition.
 */

#include <cannon/math/random_double.hpp>

namespace cannon {
  namespace math {

    // Free Functions

    /*!
     * \brief Generate permutation of noise.
     *
     * \returns Permutation of length Perlin::point_count_;
     */
    int* perlin_generate_perm();

    /*!
     * \brief Permute the input int array of length n.
     * 
     * \param p The array to permute.
     * \param n The length of the input array.
     */
    void permute(int* p, int n);

    /*!
     * \brief Compute trilinear interpolation of the input array.
     *
     * \param c Array to interpolate.
     * \param u First interpolation coefficient.
     * \param v Second interpolation coefficient.
     * \param w Third interpolation coefficient.
     *
     * \returns The interpolant.
     */
    double trilinear_interp(double c[2][2][2], double u, double v, double w);

    /*!
     * \brief Compute Perlin trilinear interpolation of the input array.
     *
     * \param c Array to interpolate.
     * \param u First interpolation coefficient.
     * \param v Second interpolation coefficient.
     * \param w Third interpolation coefficient.
     *
     * \returns The interpolant.
     */
    double trilinear_interp(Vector3d c[2][2][2], double u, double v, double w);

    /*!
     * \brief Perform hermitian smoothing on the input.
     *
     * \param x Input number.
     *
     * \returns Hermitian smoothed input.
     */
    inline double hermitian_smooth(double x);

    /*!
     * \brief Class representing a perlin noise generator.
     */
    class Perlin {
      public:

        /*!
         * \brief Default constructor.
         */
        Perlin() {
          ranvec_ = new Vector3d[point_count_];
          for (int i = 0; i < point_count_; i++) {
            ranvec_[i] = random_unit_vec();
          }

          perm_x_ = perlin_generate_perm();
          perm_y_ = perlin_generate_perm();
          perm_z_ = perlin_generate_perm();
        }

        /*!
         * \brief Destructor.
         */
        ~Perlin() {
          delete[] ranvec_;
          delete[] perm_x_;
          delete[] perm_y_;
          delete[] perm_z_;
        }

        /*!
         * \brief Get Perlin noise value for an input 3D point.
         *
         * \param p The point to evaluate.
         *
         * \returns Noise value.
         */
        double noise(const Vector3d& p) const;

        /*!
         * \brief Get Perlin noise turbulence.
         *
         * \param p The point to evaluate.
         * \param depth Depth of turbulence (via repeated noise application)
         *
         * \returns Turbulence value
         */
        double turbulence(const Vector3d& p, int depth=7) const;

        static const int point_count_ = 256; //!< Number of random floats to generate.

      private:
        Vector3d* ranvec_; //!< Array of cached random vectors.
        int* perm_x_; //!< X-axis permutation.
        int* perm_y_; //!< Y-axis permutation.
        int* perm_z_; //!< Z-axis permutation.
    };


  } // namespace math
} // namespace cannon

#endif /* ifndef CANNON_MATH_PERLIN_H */
