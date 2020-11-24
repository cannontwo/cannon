#include <catch2/catch.hpp>
#include <thirdparty/approval_tests/ApprovalTests.hpp>

#include <cannon/graphics/viewer_3d.hpp>
#include <cannon/graphics/opengl_writer.hpp>

using namespace cannon::graphics;
using namespace ApprovalTests;

TEST_CASE("Viewer3D", "[graphics]") {
  Viewer3D v;
  
  // Set up test render scene
  v.spawn_model("assets/test/test.obj");
  v.c.set_pos({5.0, 5.0, 10.0});
  v.c.set_direction({0.5, 0.75, 1.0});

  v.render_loop([&]() {
        static int i = 0;
        i++;

        if (i == 20) {
          OpenGLWriter writer(v);
          Approvals::verify(writer);
        }
      });

}
