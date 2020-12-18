#ifdef CANNON_BUILD_GRAPHICS

#include <catch2/catch.hpp>

#include <cannon/graphics/cubemap.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

TEST_CASE("Cubemap", "[graphics]") {
  Window w;

  OpenGLState s;

  Cubemap c;

  OpenGLState s1;
  REQUIRE(s == s1);

  c.bind(GL_TEXTURE0);
  OpenGLState s2;
  REQUIRE(s2.active_texture == GL_TEXTURE0);
  REQUIRE(s2.cubemap_texture != 0);

  c.unbind();
  OpenGLState s3;
  REQUIRE(s3 == s);
}
#endif
