#ifdef CANNON_BUILD_GRAPHICS
#include <catch2/catch.hpp>
#include <thirdparty/approval_tests/ApprovalTests.hpp>

#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_writer.hpp>

using namespace cannon::graphics;
using namespace ApprovalTests;

TEST_CASE("TextRenderer", "[graphics]") {
  {
  Window w;
  
  // TODO Set up test render scene

  w.display_text(25.0, 25.0, 1.0, "Hello, world!", 
      [](OverlayText& o) {
        std::stringstream ss;
        ss << "Hello, world";
        o.text.replace(o.text.begin(), o.text.end(), ss.str()); 
      });
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
