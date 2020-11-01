#ifndef CANNON_GRAPHICS_PROJECTION_H
#define CANNON_GRAPHICS_PROJECTION_H 

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace graphics {

    float to_radians(float degrees);

    Matrix4f make_perspective_frustum(float left, float right, float bottom,
        float top, float near, float far);

    // Note fov in radians
    Matrix4f make_perspective_fov(float fov, float aspect, float near, float far);
    Matrix4f make_orthographic(float left, float right, float bottom, float
        top, float near, float far);

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_PROJECTION_H */
