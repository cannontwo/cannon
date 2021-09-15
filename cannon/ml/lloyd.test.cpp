#include <catch2/catch.hpp>

#include <GLFW/glfw3.h>

#include <cannon/ml/lloyd.hpp>
#include <cannon/log/registry.hpp>

#ifdef CANNON_BUILD_GRAPHICS
#include <cannon/plot/plotter.hpp>
using namespace cannon::plot;
#endif

using namespace cannon::ml;
using namespace cannon::log;

TEST_CASE("Lloyd", "[ml]") {
  Matrix2Xd pts = Matrix2Xd::Random(2, 50);

  double update_rate = 0.1;
  unsigned int iters = 0;

#ifdef CANNON_BUILD_GRAPHICS
  Plotter plotter;
  plotter.render([&]() {
      static double last_update_time = glfwGetTime();

      if (glfwGetTime() > (last_update_time + update_rate)) {
        //do_lloyd_iteration(pts, [](const Vector2d& q){
        //    return std::abs(q[0]) < 0.5 ? q.norm() + 0.1 : 0.1;
        //    }, LloydSamplingStrategy::Grid);
        
        MatrixXd old_pts = pts;

        do_lloyd_iteration(pts, LloydSamplingStrategy::Uniform);
          
        ++iters;
        log_info(iters, "iterations of Lloyd's algorithm performed");
        log_info("Point difference is", (pts - old_pts).norm());

        plotter.clear();
        plotter.plot_points(pts.transpose().cast<float>());
        last_update_time = glfwGetTime();
      }
  });
#endif
}
