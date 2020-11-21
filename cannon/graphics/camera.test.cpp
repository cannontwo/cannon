#include <cassert>

#include <cannon/graphics/window.hpp>
#include <cannon/graphics/camera.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

int main() {
  Window w;
  OpenGLState start_state;

  Camera c({0.0, 0.0, 1.0}, {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0});

  assert(c.get_pos() == Vector3f(0.0, 0.0, 1.0));
  assert(c.get_direction() == Vector3f(0.0, 0.0, 1.0));
  assert(c.get_right() == Vector3f(1.0, 0.0, 0.0));

  Matrix4f tmp;
  tmp << 1.0, 0.0, 0.0, 0.0,
         0.0, 1.0, 0.0, 0.0,
         0.0, 0.0, 1.0, -1.0,
         0.0, 0.0, 0.0, 1.0;

  assert(c.get_view_mat() == tmp);

  OpenGLState end_state;
  assert(start_state == end_state);
}
