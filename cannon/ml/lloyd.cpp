#include <cannon/ml/lloyd.hpp>

#include <stdexcept>

#include <cannon/geom/kd_tree_indexed.hpp>
#include <cannon/math/random_double.hpp>

using namespace cannon::ml;
using namespace cannon::geom;
using namespace cannon::math;

void cannon::ml::do_lloyd_iteration(Matrix2Xd &pts, LloydSamplingStrategy strat,
                                    double x_low, double x_high, double y_low,
                                    double y_high) {
  do_lloyd_iteration(pts, [](const Vector2d &) { return 1.0; }, strat, x_low,
                     x_high, y_low, y_high);
}

void cannon::ml::do_lloyd_iteration(Matrix2Xd &pts,
                                    std::function<double(const Vector2d &)> f,
                                    LloydSamplingStrategy strat, double x_low,
                                    double x_high, double y_low,
                                    double y_high) {
  KDTreeIndexed tree(2);
  tree.insert(pts);

  pts = MatrixXd::Zero(pts.rows(), pts.cols());
  std::vector<double> total_weights(pts.cols(), 0.0);

  double x_step, y_step;
  Vector2d query;
  switch (strat) {
    case LloydSamplingStrategy::Grid:
      static const unsigned int grid_dim = 100;
      x_step = (x_high - x_low) / static_cast<double>(grid_dim);
      y_step = (y_high - y_low) / static_cast<double>(grid_dim);
      
      for (unsigned int i = 0; i < grid_dim; ++i) {
        for (unsigned int j = 0; j < grid_dim; ++j) {
          query[0] = x_low + (i * x_step);
          query[1] = y_low + (j * y_step);

          int idx = tree.get_nearest_idx(query);
          double weight = f(query);
          Vector2d old_total = pts.col(idx) * total_weights[idx];
          total_weights[idx] += weight;

          pts.col(idx) = (old_total + weight * query) / total_weights[idx];
        }
      }
      break;
    case LloydSamplingStrategy::Uniform:
      static const unsigned int num_samples = 1e4;
      for (unsigned int i = 0; i < num_samples; i++) {
          query[0] = random_double(x_low, x_high);
          query[1] = random_double(y_low, y_high);

          // TODO Extract somehow (shared)
          int idx = tree.get_nearest_idx(query);
          double weight = f(query);
          Vector2d old_total = pts.col(idx) * total_weights[idx];
          total_weights[idx] += weight;

          pts.col(idx) = (old_total + weight * query) / total_weights[idx];
      }
      break;
  }
}

