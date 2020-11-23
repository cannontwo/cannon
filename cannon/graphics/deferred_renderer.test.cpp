#include <catch2/catch.hpp>

#include <cannon/graphics/deferred_renderer.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

TEST_CASE("DeferredRenderer", "[graphics]") {
  DeferredRenderer d;
  OpenGLState s;
}
