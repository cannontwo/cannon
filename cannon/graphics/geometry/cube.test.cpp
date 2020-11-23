#include <cannon/graphics/geometry/cube.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

int main() {
  Window w;
  OpenGLState s;

  geometry::Cube c;
  OpenGLState s1;
  assert(s == s1);

  Matrix4f tmp = Matrix4f::Zero();
  c.draw(tmp, tmp);
  OpenGLState s2;
  assert(s == s2);

  auto p = std::make_shared<ShaderProgram>();
  p->attach_vertex_shader("shaders/mvp_uniform_color.vert");
  p->attach_fragment_shader("shaders/pass_color.frag");
  p->link();

  c.draw(p, tmp, tmp);
  OpenGLState s3;
  assert(s == s3);
}
