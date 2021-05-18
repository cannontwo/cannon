#ifndef CANNON_MATH_BARYCENTRIC_COORDS_H
#define CANNON_MATH_BARYCENTRIC_COORDS_H 

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace math {

    Vector3d compute_barycentric_coords(const Vector3d& v0, const Vector3d& v1,
        const Vector3d& v2, const Vector3d& p);

  } // namespace math
} // namespace cannon
#endif /* ifndef CANNON_MATH_BARYCENTRIC_COORDS_H */
