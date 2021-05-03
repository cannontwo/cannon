#include <cannon/math/perlin.hpp>

using namespace cannon::math;

double Perlin::noise(const Vector3d& p) const {
  auto i = static_cast<int>(4 * p.x()) & 255;
  auto j = static_cast<int>(4 * p.y()) & 255;
  auto k = static_cast<int>(4 * p.z()) & 255;

  return ranfloat_[perm_x_[i] ^ perm_y_[j] ^ perm_z_[k]];
}

// Free Functions
int* cannon::math::perlin_generate_perm() {
  auto p = new int[Perlin::point_count_];

  for (int i = 0; i < Perlin::point_count_; i++) {
    p[i] = i;
  }

  permute(p, Perlin::point_count_);
  return p;
}

void cannon::math::permute(int* p, int n) {
  for (int i = n - 1; i > 0; i--) {
    int target = static_cast<int>(random_double(0, i+1));
    int tmp = p[i];
    p[i] = p[target];
    p[target] = tmp;
  }
}
