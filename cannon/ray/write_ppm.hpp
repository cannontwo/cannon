#ifndef CANNON_RAY_WRITE_PPM
#define CANNON_RAY_WRITE_PPM

#include <iostream>

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace ray {

    void write_color(std::ostream& os, const Vector3d& pixel_color);

  }
}

#endif /* ifndef CANNON_RAY_WRITE_PPM */
