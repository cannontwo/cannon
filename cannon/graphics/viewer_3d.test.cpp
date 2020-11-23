#include <catch2/catch.hpp>
#include <thirdparty/approval_tests/ApprovalTests.hpp>

#include <cannon/graphics/viewer_3d.hpp>
#include <cannon/graphics/opengl_writer.hpp>

using namespace cannon::graphics;
using namespace ApprovalTests;

TEST_CASE("Viewer3D", "[graphics]") {
  Viewer3D v;
  
  // TODO Set up test render scene

  v.render_loop([&]() {
        static int i = 0;
        i++;

        if (i == 4) {
          OpenGLWriter writer(v);
          Approvals::verify(writer);
        }
      });

}
