#include <catch2/catch.hpp>

#include <cannon/geom/graph.hpp>
#include <cannon/log/registry.hpp>

#ifdef CANNON_BUILD_GRAPHICS
#include <cannon/plot/plotter.hpp>

using namespace cannon::plot;
#endif

using namespace cannon::geom;
using namespace cannon::log;

unsigned int node_id(unsigned int cols, unsigned int i, unsigned int j) {
  return j * cols + i;
}

Graph construct_grid(unsigned int rows, unsigned int cols) {
  Graph g;

  for (unsigned int i = 0; i < cols; ++i) {
    for (unsigned int j = 0; j < rows; ++j) {
      unsigned int current = node_id(cols, i, j);

      if (i > 0)
        g.add_edge(current, node_id(cols, i - 1, j), 1.0);

      if (i < cols - 1)
        g.add_edge(current, node_id(cols, i + 1, j), 1.0);

      if (j > 0)
        g.add_edge(current, node_id(cols, i, j - 1), 1.0);

      if (j < rows - 1)
        g.add_edge(current, node_id(cols, i, j + 1), 1.0);
    }
  }

  return g;
}

TEST_CASE("Graph", "[geom]") {
  unsigned int rows = 10;
  unsigned int cols = 10;
  Graph g = construct_grid(rows, cols);

  std::vector<std::pair<double, double>> locs(rows * cols);

  for (unsigned int i = 0; i < cols; ++i) {
    for (unsigned int j = 0; j < rows; ++j) {
      locs[node_id(cols, i, j)] = std::make_pair(static_cast<double>(i), static_cast<double>(j));
    }
  }
  
  auto path = g.astar(0, cols*rows - 1, [&](unsigned int v) {
    double dx = locs[cols*rows - 1].first - locs[v].first;
    double dy = locs[cols*rows - 1].second - locs[v].second;

    return std::sqrt(dx * dx + dy * dy);
  });

  REQUIRE(path[0] == 0);
  REQUIRE(path[path.size()-1] == cols*rows - 1);

  log_info("Shortest path from 0 to 8 is:");
  for (auto& v : path) {
    log_info("\t", v);
  }

#ifdef CANNON_BUILD_GRAPHICS
  Plotter plotter;

  std::vector<Vector2d> pts;
  for (auto& v : path) {
    pts.push_back({locs[v].first, locs[v].second});
  }

  plotter.plot(pts);

  MatrixX2f plot_pts = MatrixX2f::Zero(cols * rows, 2);
  for (unsigned int i = 0; i < g.num_vertices(); ++i) {
    plot_pts(i, 0) = locs[i].first;
    plot_pts(i, 1) = locs[i].second;
  }

  plotter.plot_points(plot_pts);

  //plotter.render();
#endif
}
