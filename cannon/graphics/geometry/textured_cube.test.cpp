#ifdef CANNON_BUILD_GRAPHICS
#include <catch2/catch.hpp>

#include <cannon/graphics/geometry/textured_cube.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

TEST_CASE("TexturedCube", "[graphics]") {
  Window w;
  OpenGLState s;

  auto textured_program = std::make_shared<ShaderProgram>("textured_cube_shader");
  textured_program->attach_vertex_shader("shaders/mvp_normals_tex.vert");
  textured_program->attach_fragment_shader("shaders/material_lights_tex.frag");
  textured_program->link();

  geometry::TexturedCube c(textured_program);
  OpenGLState s1;
  REQUIRE(s == s1);

  Matrix4f tmp = Matrix4f::Zero();
  c.draw(tmp, tmp);
  OpenGLState s2;
  REQUIRE(s == s2);

  auto p = std::make_shared<ShaderProgram>();
  p->attach_vertex_shader("shaders/mvp_uniform_color.vert");
  p->attach_fragment_shader("shaders/pass_color.frag");
  p->link();

  c.draw(p, tmp, tmp);
  OpenGLState s3;
  REQUIRE(s == s3);
}
#endif
