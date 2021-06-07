#ifndef CANNON_ML_OPTIMIZATION_RESULT_H
#define CANNON_ML_OPTIMIZATION_RESULT_H 

/*!
 * \file cannon/ml/optimization_result.hpp
 * \brief File containing OptimizationResult class definition.
 */

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace ml {

    /*!
     * \brief Class representing the result of an optimization attempt.
     */
    class OptimizationResult {
      public:
        OptimizationResult() = delete;

        /*!
         * \brief Constructor initializing all member variables.
         */
        OptimizationResult(double objective, const VectorXd& solution, 
            unsigned int iterations, bool max_iterations) : objective(objective),
        solution(solution), iterations(iterations), max_iterations(max_iterations) {}

        double objective; //!< Value of objective at solution, if found
        VectorXd solution; //!< Solution state, if found
        unsigned int iterations; //!< Number of iterations executed while optimizing
        bool max_iterations; //!< Whether the maximum number of iterations was exceeded
    };

    std::ostream& operator<<(std::ostream& os, OptimizationResult result);

  } // namespace ml
} // namespace cannon

#endif /* ifndef CANNON_ML_OPTIMIZATION_RESULT_H */
