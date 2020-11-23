#include <catch2/catch.hpp>

#include <cannon/graphics/window.hpp>
#include <cannon/graphics/font.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

TEST_CASE("Font", "[graphics]") {
  Window w;
  OpenGLState s;

  Font f;
  OpenGLState s1;
  REQUIRE(s == s1);
}
