#include <cannon/math/barycentric_coords.hpp>

using namespace cannon::math;

Vector3d cannon::math::compute_barycentric_coords(const Vector3d& v0, const Vector3d& v1,
    const Vector3d& v2, const Vector3d& p) {
  Matrix4d plane_mat;
  plane_mat << v0[0], v1[0], v2[0], p[0],
               v0[1], v1[1], v2[1], p[1],
               v0[2], v1[2], v2[2], p[2],
               1.0, 1.0, 1.0, 1.0;
  if (!nearly_equal(plane_mat.determinant(), 0.0))
    throw std::runtime_error("Passed query point not in same plane as triangle");

  Vector3d v0v1 = v1 - v0;
  Vector3d v0v2 = v2 - v0;

  // Compute normal vector to triangle
  Vector3d n = v0v1.cross(v0v2);
  double denom = n.dot(n);

  // First coord
  Vector3d v1v2 = v2 - v1;
  Vector3d pv1 = p - v1;
  Vector3d c = v1v2.cross(pv1);
  double u = n.dot(c);
  u = u / denom;
  
  // Second coord
  Vector3d v2v0 = v0 - v2;
  Vector3d pv2 = p - v2;
  c = v2v0.cross(pv2);
  double v = n.dot(c);
  v = v / denom;

  // Final coordinate 
  double w = 1 - (u + v);

  return {u, v, w};
}
