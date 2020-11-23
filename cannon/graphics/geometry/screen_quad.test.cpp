#include <cannon/graphics/geometry/screen_quad.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_state.hpp>
#include <cannon/graphics/texture.hpp>

using namespace cannon::graphics;

int main() {
  Window w;
  OpenGLState s;

  auto p = std::make_shared<ShaderProgram>();
  p->attach_vertex_shader("shaders/mvp_uniform_color.vert");
  p->attach_fragment_shader("shaders/pass_color.frag");
  p->link();

  auto t = std::make_shared<Texture>();
  geometry::ScreenQuad q(t);
  OpenGLState s1;
  assert(s == s1);

  q.resize(100, 100);
  OpenGLState s2;
  assert(s == s2);

  q.draw();
  OpenGLState s3;
  assert(s == s3);

  q.draw(p);
  OpenGLState s4;
  assert(s == s4);

  std::vector<std::shared_ptr<Texture>> st;
  st.push_back(t);

  q.draw(p, st);
  OpenGLState s5;
  assert(s == s5);

  q.set_tex(t);
  OpenGLState s6;
  assert(s == s6);
}
