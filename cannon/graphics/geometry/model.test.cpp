#ifdef CANNON_BUILD_GRAPHICS
#include <catch2/catch.hpp>

#include <cannon/graphics/geometry/model.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

TEST_CASE("Model", "[graphics]") {
  Window w;
  OpenGLState s;

  auto p = std::make_shared<ShaderProgram>();
  p->attach_vertex_shader("shaders/mvp_uniform_color.vert");
  p->attach_fragment_shader("shaders/pass_color.frag");
  p->link();

  geometry::Model m(p, "assets/backpack/backpack.obj");
  OpenGLState s1;
  REQUIRE(s == s1);

  Matrix4f tmp = Matrix4f::Zero();
  m.draw(tmp, tmp);
  OpenGLState s2;
  REQUIRE(s == s2);

  m.draw(p, tmp, tmp);
  OpenGLState s3;
  REQUIRE(s == s3);
}
#endif
