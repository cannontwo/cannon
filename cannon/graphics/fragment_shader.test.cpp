#include <catch2/catch.hpp>

#include <cannon/graphics/fragment_shader.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

TEST_CASE("FragmentShader", "[graphics]") {
  Window w;
  OpenGLState s;

  const char *str = BASIC_FRAGMENT_SHADER.c_str();

  FragmentShader f(&str);
  OpenGLState s1;
  REQUIRE(s == s1);
}
