#include <catch2/catch.hpp>

#include <cannon/graphics/geometry/screen_quad.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_state.hpp>
#include <cannon/graphics/texture.hpp>

using namespace cannon::graphics;

TEST_CASE("ScreenQuad", "[graphics]") {
  Window w;
  OpenGLState s;

  auto p = std::make_shared<ShaderProgram>();
  p->attach_vertex_shader("shaders/mvp_uniform_color.vert");
  p->attach_fragment_shader("shaders/pass_color.frag");
  p->link();

  auto t = std::make_shared<Texture>();
  geometry::ScreenQuad q(t);
  OpenGLState s1;
  REQUIRE(s == s1);

  q.resize(100, 100);
  OpenGLState s2;
  REQUIRE(s == s2);

  q.draw();
  OpenGLState s3;
  REQUIRE(s == s3);

  q.draw(p);
  OpenGLState s4;
  REQUIRE(s == s4);

  std::vector<std::shared_ptr<Texture>> st;
  st.push_back(t);

  q.draw(p, st);
  OpenGLState s5;
  REQUIRE(s == s5);

  q.set_tex(t);
  OpenGLState s6;
  REQUIRE(s == s6);
}
