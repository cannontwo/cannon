#ifndef CANNON_MATH_FFT_H
#define CANNON_MATH_FFT_H 

/*!
 * \file cannon/math/fft.hpp
 * \brief File containing utilities for computing various kinds of Fast Fourier
 * Transform.
 */

#include <vector>
#include <complex>

namespace cannon {
  namespace math {

    /*!
     * \brief Helper function that swaps elements in the input array so that
     * the indices of elements are bit-reversed. 
     *
     * \param data The data to rearrange in bit-reversed order. Will be modified.
     */
    void bit_reverse(std::vector<std::complex<double>>& data);

    /*!
     * \brief Function that computes a Fast Fourier Transform or its inverse of
     * the input data. If the inverse transform is requested, then the cyclic
     * layout for frequence data suggested in Numerical Recipes pg. 612 is
     * expected. Note that this is the lowest-level routine, so data should
     * have a power of two length.
     * 
     * \param data The sampled time series or frequency data to transform.
     * Should have length that is a power of two. 
     * \param inverse Whether to compute the forward or inverse transform.
     *
     * \returns The transformed data.
     */
    std::vector<std::complex<double>> fft(std::vector<std::complex<double>> data,
                                          bool inverse = false);

  } // namespace math
} // namespace cannon

#endif /* ifndef CANNON_MATH_FFT_H */
