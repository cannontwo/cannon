#ifdef CANNON_BUILD_GRAPHICS
#include <catch2/catch.hpp>
#include <thirdparty/approval_tests/ApprovalTests.hpp>

#include <cannon/plot/plotter.hpp>
#include <cannon/graphics/opengl_writer.hpp>

using namespace cannon::graphics;
using namespace cannon::plot;
using namespace ApprovalTests;

TEST_CASE("Line", "[plot]") {
  Plotter p;

  MatrixX2f pts(2, 2);
  pts << -5.0 * std::cos(M_PI / 4.0), -5.0 * std::sin(M_PI / 4.0),
          5.0 * std::cos(M_PI / 4.0), 5.0 * std::sin(M_PI / 4.0);

  std::shared_ptr<Line> l = p.plot_line(pts, Vector4f(0.0, 0.0, 0.0, 1.0));

  p.set_xlim(-5.0, 5.0);
  p.set_ylim(-5.0, 5.0);

  p.render([&]() {
        static int i = 0;
        i++;

        if (i == 20) {
          OpenGLWriter writer(p);
          Approvals::verify(writer);
        }
      });
}
#endif
