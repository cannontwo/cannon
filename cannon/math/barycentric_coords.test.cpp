#include <cannon/math/barycentric_coords.hpp>

using namespace cannon::math;

int main() {
  Vector3d v0;
  v0 << 0.0,
        0.0,
        0.0;

  Vector3d v1;
  v1 << 1.0,
        0.0,
        0.0;
  
  Vector3d v2;
  v2 << 0.0,
        1.0,
        0.0;

  Vector3d p;
  p << 0.5,
       0.5,
       0.0;

  Vector3d test_bary;
  test_bary << 0.0,
               0.5,
               0.5;

  assert(compute_barycentric_coords(v0, v1, v2, p) == test_bary);

  // Should throw exception, since p is non-planar
  p << 0.5,
       0.5,
       1.0;
  try {
    compute_barycentric_coords(v0, v1, v2, p);
    assert(false); 
  } catch (...) {}
}
