#ifndef CANNON_ML_BFGS_H
#define CANNON_ML_BFGS_H 

/*!
 * \file cannon/ml/bfgs.hpp
 * \brief File containing BFGSOptimizer class definition.
 */

#include <functional>

#include <Eigen/Dense>

#include <cannon/utils/class_forward.hpp>

using namespace Eigen;

using RealFunc = std::function<double(const VectorXd&)>;
using MultiFunc = std::function<VectorXd(const VectorXd&)>;

namespace cannon {
  namespace ml {

    CANNON_CLASS_FORWARD(OptimizationResult);

    /*!
     * \brief Class representing a BFGS optimizer, which is a quasi-Newton
     * unconstrained optimization method for nonlinear problems. See
     * https://en.wikipedia.org/wiki/Broyden%E2%80%93Fletcher%E2%80%93Goldfarb%E2%80%93Shanno_algorithm
     */
    class BFGSOptimizer {
      public:
        BFGSOptimizer() = delete;

        /*!
         * \brief Constructor taking a function to optimize and a function
         * providing gradients for that function.
         */
        BFGSOptimizer(RealFunc f, MultiFunc f_grad) : f_(f), f_grad_(f_grad) {}

        /*!
         * \brief Minimize the function stored by this object, beginning from
         * the input start state.
         *
         * \param start Initial state for optimization
         * \param eps Small number used to detect convergence
         * \param iterations Maximum number of iterations to perform
         *
         * \returns The result of the optimization.
         */ 
        OptimizationResult optimize(const VectorXd& start, double eps=1e-4, unsigned
            int iterations=100);

      private:
        RealFunc f_; //!< The function to minimize
        MultiFunc f_grad_; //!< Gradient function for the function to minimize

    };

  } // namespace ml
} // namespace cannon

#endif /* ifndef CANNON_ML_BFGS_H */
