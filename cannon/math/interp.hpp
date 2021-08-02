#ifndef CANNON_MATH_INTERP_H
#define CANNON_MATH_INTERP_H 

/*!
 * \file cannon/math/interp.hpp
 * \brief File containing utilities for doing function interpolation.
 */

#include <vector>
#include <functional>

namespace cannon {
  namespace math {

    /*!
     * \brief Compute the Chebyshev points between the input bounds. See
     * https://en.wikipedia.org/wiki/Chebyshev_nodes 
     *
     * \param num Number of points to generate
     * \param low Lower limit for points
     * \param high Upper limit for points
     *
     * \returns A vector containing the Chebyshev points.
     */
    std::vector<double> cheb_points(unsigned int num, double low=-1.0, double high=1.0);

    /*!
     * \brief Compute the Lagrange interpolant for the input function on the
     * input points.
     *
     * \param f The function to interpolate.
     * \param pts The points to interpolate on.
     *
     * \returns The Lagrange interpolating polynomial on the input points.
     */
    std::function<double(double)>
    lagrange_interp(std::function<double(double)> f, const std::vector<double>& pts);
  }
} 

#endif /* ifndef CANNON_MATH_INTERP_H */
