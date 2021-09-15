#ifndef CANNON_ML_LLOYD_H
#define CANNON_ML_LLOYD_H 

/*!
 * \file cannon/ml/lloyd.hpp
 * \brief File containing functions relating to Lloyd's algorithm for Voronoi
 * relaxation.
 */

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace ml {

    /*!
     * \brief Enum class representing the different ways that the centroid
     * integral used by Lloyd's algorithm can be numerically approximated.
     */
    enum class LloydSamplingStrategy {
      Grid,
      Uniform,
    };

    /*!
     * \brief Function to compute a single iteration of the classic Lloyd's
     * algorithm in place within the specified bounds.
     *
     * \param pts The current points making up the Voronoi diagram.
     * \param strat Sampling strategy to use for approximating centroid
     * \param x_low X axis lower bound
     * \param x_high X axis upper bound
     * \param y_low Y axis lower bound
     * \param y_high Y axis upper bound
     */
    void do_lloyd_iteration(
        Matrix2Xd &pts,
        LloydSamplingStrategy strat = LloydSamplingStrategy::Grid,
        double x_low = -1.0, double x_high = 1.0, double y_low = -1.0,
        double y_high = 1.0);

    /*!
     * \brief Function to compute a single iteration of Lloyd's
     * algorithm with respect to the input weighting function in place within
     * the specified bounds.
     *
     * \param pts The current points making up the Voronoi diagram.
     * \param f The weighting function to use. Should be everywhere positive.
     * \param strat Sampling strategy to use for approximating centroid
     * \param x_low X axis lower bound
     * \param x_high X axis upper bound
     * \param y_low Y axis lower bound
     * \param y_high Y axis upper bound
     */
    void do_lloyd_iteration(
        Matrix2Xd &pts, std::function<double(const Vector2d&)> f,
        LloydSamplingStrategy strat = LloydSamplingStrategy::Grid,
        double x_low = -1.0, double x_high = 1.0, double y_low = -1.0,
        double y_high = 1.0);
  }
}

#endif /* ifndef CANNON_ML_LLOYD_H */
