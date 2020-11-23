#include <cannon/graphics/geometry/sdf_volume.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

int main() {
  Window w;
  OpenGLState s;

  auto p = std::make_shared<ShaderProgram>();
  p->attach_vertex_shader("shaders/mvp_uniform_color.vert");
  p->attach_fragment_shader("shaders/pass_color.frag");
  p->link();

  geometry::SDFVolume v(p);
  OpenGLState s1;
  assert(s == s1);

  Matrix4f tmp = Matrix4f::Zero();
  v.draw(tmp, tmp);
  OpenGLState s2;
  assert(s == s2);

  v.draw(p, tmp, tmp);
  OpenGLState s3;
  assert(s == s3);
}
