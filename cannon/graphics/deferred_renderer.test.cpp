#ifdef CANNON_BUILD_GRAPHICS

#include <catch2/catch.hpp>
#include <thirdparty/approval_tests/ApprovalTests.hpp>

#include <cannon/graphics/deferred_renderer.hpp>
#include <cannon/graphics/opengl_state.hpp>
#include <cannon/graphics/opengl_writer.hpp>

using namespace cannon::graphics;

TEST_CASE("DeferredRenderer", "[graphics]") {
  DeferredRenderer d;
  OpenGLState s;

  d.viewer.spawn_model("assets/test/test.obj");
  d.viewer.c.set_pos({5.0, 5.0, 10.0});
  d.viewer.c.set_direction({0.5, 0.75, 1.0});

  // TODO Set up test render scene

  d.render_loop([&]() {
        static int i = 0;
        i++;

        if (i == 5) {
          OpenGLWriter writer(d.viewer);
          Approvals::verify(writer);
        }
      });
}
#endif
