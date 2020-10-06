#include <cmath>

#include <cannon/graphics/projection.hpp>

using namespace cannon::graphics;

Matrix4f cannon::graphics::make_perspective_frustum(float left, float right, float bottom,
    float top, float near, float far) {
  Matrix4f m;
  m << 2 * near / (right - left), 0.0f, (right + left) / (right - left), 0.0f,
       0.0f, 2 * near / (top - bottom), (top + bottom) / (top - bottom), 0.0f,
       0.0f, 0.0f, -(far + near) / (far - near), -(2 * far * near) / (far - near),
       0.0f, 0.0f, -1.0f, 0.0f;

  return m;
}

Matrix4f cannon::graphics::make_perspective_fov(float fov, float aspect, float near, float far) {
  float tangent = std::tan(fov/2.0);
  float height = near * tangent;
  float width = height * aspect;

  return make_perspective_frustum(-width, width, -height, height, near, far);
}

Matrix4f cannon::graphics::make_orthographic(float left, float right, float
    bottom, float top, float near, float far) {
  Matrix4f m;
  m << 2 / (right - left), 0.0f, 0.0f, -(right + left) / (right - left),
       0.0f, 2 / (top - bottom), 0.0f, -(top + bottom) / (top - bottom),
       0.0f, 0.0f, -2 / (far - near), -(far + near) / (far - near),
       0.0f, 0.0f, 0.0f, 1.0f;

  return m;
}
