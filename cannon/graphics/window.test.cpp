#include <catch2/catch.hpp>
#include <thirdparty/approval_tests/ApprovalTests.hpp>

#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_writer.hpp>

using namespace cannon::graphics;
using namespace ApprovalTests;

TEST_CASE("Window", "[graphics]") {
  Window w;
  
  // TODO Set up test render scene

  w.render_loop([&]() {
        static int i = 0;
        i++;

        if (i == 20) {
          OpenGLWriter writer(w);
          Approvals::verify(writer);
        }
      });
}
