#ifdef CANNON_BUILD_GRAPHICS
#include <catch2/catch.hpp>
#include <thirdparty/approval_tests/ApprovalTests.hpp>

#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_writer.hpp>

using namespace cannon::graphics;
using namespace ApprovalTests;

TEST_CASE("Window", "[graphics]") {
  GLFWwindow *gl_window;
  {
  Window w;
  gl_window = w.get_gl_window();
  
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
  //terminate_opengl_context(gl_window);
}
#endif
