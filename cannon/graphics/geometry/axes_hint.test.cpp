#include <cannon/graphics/geometry/axes_hint.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

int main() {
  Window w;
  OpenGLState s;

  geometry::AxesHint ah;
  OpenGLState s1;
  assert(s == s1);

  Matrix4f tmp = Matrix4f::Zero();
  ah.draw(tmp, tmp);
  OpenGLState s2;
  assert(s == s2);

  auto p = std::make_shared<ShaderProgram>();
  p->attach_vertex_shader("shaders/pass_pos.vert");
  p->attach_fragment_shader("shaders/axes_hint.frag");
  p->link();

  ah.draw(p, tmp, tmp);
  OpenGLState s3;
  assert(s == s3);
}
