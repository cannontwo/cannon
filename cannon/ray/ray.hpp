#ifndef CANNON_RAY_RAY_H
#define CANNON_RAY_RAY_H 

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace ray {
    
    class Ray {
      public:
        Ray() {}

        Ray(const Vector3d& origin, const Vector3d& direction) : orig_(origin), dir_(direction) {}

        Vector3d at(double t) const;

      public:
        Vector3d orig_;
        Vector3d dir_;

    };

  }
}

#endif /* ifndef CANNON_RAY_RAY_H */
