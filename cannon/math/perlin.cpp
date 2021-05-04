#include <cannon/math/perlin.hpp>

using namespace cannon::math;

double Perlin::noise(const Vector3d& p) const {
  auto u = p.x() - std::floor(p.x());
  auto v = p.y() - std::floor(p.y());
  auto w = p.z() - std::floor(p.z());

  auto i = static_cast<int>(std::floor(p.x()));
  auto j = static_cast<int>(std::floor(p.y()));
  auto k = static_cast<int>(std::floor(p.z()));

  Vector3d c[2][2][2];
  for (int di = 0; di < 2; di++) {
    for (int dj = 0; dj < 2; dj++) {
      for (int dk = 0; dk < 2; dk++) {
        c[di][dj][dk] = ranvec_[perm_x_[(i+di) & 255] ^ perm_y_[(j+dj) & 255]
          ^ perm_z_[(k+dk) & 255]];
      }
    }
  }

  return trilinear_interp(c, u, v, w);
}

double Perlin::turbulence(const Vector3d& p, int depth) const {
  auto accum = 0.0;
  auto weight = 1.0;
  Vector3d temp_p = p;

  for (int i = 0; i < depth; i++) {
    accum += weight * noise(temp_p);
    weight *= 0.5;
    temp_p *= 2;
  }

  return std::fabs(accum);
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

double cannon::math::trilinear_interp(double c[2][2][2], double u, double v, double w) {
  double accum = 0.0;

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
        accum += (i*u + (1-i)*(1-u)) * 
                  (j*v + (1-j)*(1-v)) *
                  (k*w + (1-k)*(1-w)) * c[i][j][k];
      }
    }
  }

  return accum;
}

double cannon::math::trilinear_interp(Vector3d c[2][2][2], double u, double v, double w) {
  auto uu = hermitian_smooth(u);
  auto vv = hermitian_smooth(v);
  auto ww = hermitian_smooth(w);

  auto accum = 0.0;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
        Vector3d weight(u-i, v-j, w-k);

        accum += (i*uu + (1-i)*(1-uu)) * 
                 (j*vv + (1-j)*(1-vv)) *
                 (k*ww + (1-k)*(1-ww)) * c[i][j][k].dot(weight);
      }
    }
  }

  return accum;
}

inline double cannon::math::hermitian_smooth(double x) {
  return x * x * (3 - 2*x);
}
