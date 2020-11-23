#include <catch2/catch.hpp>

#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

TEST_CASE("VertexShader", "[graphics]") {
  Window w;
  OpenGLState s;

  const char *str = BASIC_VERTEX_SHADER.c_str();
  VertexShader v(&str);
  OpenGLState s1;
  REQUIRE(s == s1);
}
