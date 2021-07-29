#ifdef CANNON_BUILD_GRAPHICS
#include <catch2/catch.hpp>
#include <thirdparty/approval_tests/ApprovalTests.hpp>

#include <cannon/plot/polygon.hpp>
#include <cannon/plot/plotter.hpp>
#include <cannon/graphics/opengl_writer.hpp>

using namespace cannon::plot;
using namespace cannon::graphics;

TEST_CASE("Polygon", "[plot]") {
  Plotter p;

  std::vector<Vector2d> poly;

  poly.push_back(Vector2d(0, 0));
  poly.push_back(Vector2d(2, 0));
  poly.push_back(Vector2d(2, 2));
  poly.push_back(Vector2d(0, 2));

  Vector4f color = Vector4f::Zero();

  p.plot_polygon(poly, color);

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
