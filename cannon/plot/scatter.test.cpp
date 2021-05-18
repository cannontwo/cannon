#ifdef CANNON_BUILD_GRAPHICS
#include <catch2/catch.hpp>
#include <thirdparty/approval_tests/ApprovalTests.hpp>

#include <cannon/plot/plotter.hpp>
#include <cannon/plot/scatter.hpp>
#include <cannon/graphics/opengl_writer.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::graphics;
using namespace cannon::plot;
using namespace cannon::log;
using namespace ApprovalTests;

TEST_CASE("Scatter", "[plot]") {
  Plotter p;
  
  MatrixX2f pts = MatrixX2f::Random(1, 2);
  log_info(pts);
  auto scatter = p.plot_points(pts, Vector4f(0.0, 0.0, 0.0, 1.0), 15.0);

  double last_time = glfwGetTime();
  double time_delta = 0.1;

  p.render([&]() {
        static int i = 0;
        i++;

        if (glfwGetTime() > last_time + time_delta) {
          MatrixX2f new_pt = MatrixX2f::Random(1, 2);
          scatter->add_points(new_pt);
          last_time = glfwGetTime();
        }

        if (i == 20) {
          OpenGLWriter writer(p);
          Approvals::verify(writer);
        }
      });
}
#endif
