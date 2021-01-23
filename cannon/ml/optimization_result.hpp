#ifndef CANNON_ML_OPTIMIZATION_RESULT_H
#define CANNON_ML_OPTIMIZATION_RESULT_H 

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace ml {

    struct OptimizationResult {
      OptimizationResult() = delete;

      OptimizationResult(double objective, const VectorXd& solution, 
          unsigned int iterations, bool max_iterations) : objective(objective),
      solution(solution), iterations(iterations), max_iterations(max_iterations) {}

      double objective;
      VectorXd solution;
      unsigned int iterations;
      bool max_iterations;
    };

    std::ostream& operator<<(std::ostream& os, OptimizationResult result);

  } // namespace ml
} // namespace cannon

#endif /* ifndef CANNON_ML_OPTIMIZATION_RESULT_H */
