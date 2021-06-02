#ifndef CANNON_MATH_BARYCENTRIC_COORDS_H
#define CANNON_MATH_BARYCENTRIC_COORDS_H 

/*!
 * \file cannon/math/barycentric_coords.hpp
 * \brief File containing free functions relating to barycentric coordinate
 * calculation.
 */

/*!
 * \namespace cannon::math
 * \brief Namespace containing mathematical utilities.
 */

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace math {

    /*!
     * \brief Function to compute barycentric coordinates for a query point on
     * triangle defined by its three vertices. See
     * https://en.wikipedia.org/wiki/Barycentric_coordinate_system
     *
     * \param v0 First vertex
     * \param v1 Second vertex
     * \param v2 Third vertex
     * \param p Query point
     *
     * \returns Barycentric coordinates for the query point.
     */
    Vector3d compute_barycentric_coords(const Vector3d& v0, const Vector3d& v1,
        const Vector3d& v2, const Vector3d& p);

  } // namespace math
} // namespace cannon
#endif /* ifndef CANNON_MATH_BARYCENTRIC_COORDS_H */
