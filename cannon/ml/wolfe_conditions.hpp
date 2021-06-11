#ifndef CANNON_ML_WOLFE_CONDITIONS_H
#define CANNON_ML_WOLFE_CONDITIONS_H 

/*!
 * \file cannon/ml/wolfe_conditions.hpp
 * \brief File containing utility functions relating to line search and
 * sufficient decrease conditions.
 */

#include <functional>

#include <Eigen/Dense>

using namespace Eigen;

using RealFunc = std::function<double(const VectorXd&)>;
using MultiFunc = std::function<VectorXd(const VectorXd&)>;

namespace cannon {
  namespace ml {

    /*!
     * \brief Compute whether the input step along the input direction
     * satisfies the Armijo sufficient decrease condition with respect to the
     * function under consideration.
     *
     * \param f The function being optimized
     * \param f_grad Gradient of the function being optimized
     * \param x Current state from which to compute sufficient decrease
     * \param step Step along direction from x
     * \param direction Optimization direction
     * \param c_1 Sufficient decrease parameter
     *
     * \returns Whether the sufficient decrease condition is satisfied.
     */
    bool sufficient_decrease_condition(RealFunc f, MultiFunc f_grad, const
        VectorXd& x, double step, const VectorXd& direction, double c_1);

    /*!
     * \brief Compute whether the input step along the input direction
     * satisfies the curvature condition.
     *
     * \param f The function being optimized
     * \param f_grad Gradient of the function being optimized
     * \param x Current state from which to compute sufficient decrease
     * \param step Step along direction from x
     * \param direction Optimization direction
     * \param c_2 Curvature parameter
     */
    bool curvature_condition(RealFunc f, MultiFunc f_grad, const
        VectorXd& x, double step, const VectorXd& direction, double c_2);

    /*!
     * \brief Compute whether both Wolfe conditions are satisfied by the input
     * step along the input direction.
     *
     * \param f The function being optimized
     * \param f_grad Gradient of the function being optimized
     * \param x Current state from which to compute sufficient decrease
     * \param step Step along direction from x
     * \param direction Optimization direction
     * \param c_1 Sufficient decrease parameter
     * \param c_2 Curvature parameter
     */
    bool wolfe_conditions(RealFunc f, MultiFunc f_grad, const
        VectorXd& x, double step, const VectorXd& direction, double c_1, 
        double c_2);

    /*!
     * \brief Compute the optimal line search step between the input minimum
     * and maximum along the input direction.  Note that this is the minimizing
     * step.
     *
     * \param f The function to be minimized.
     * \param f_grad Gradient function for the function to be minimized.
     * \param x State to conduct line search from
     * \param direction Direction for line search
     * \param start_low Initial minimal step
     * \param start_high Initial maximal step
     * \param c_1 Sufficient decrease parameter
     * \param c_2 Curvature parameter
     * \param iterations Maximum number of iterations.
     */
    double line_search_zoom(RealFunc f, MultiFunc f_grad, const VectorXd& x,
        const VectorXd& direction, double start_low, double start_high,
        double c_1, double c_2, unsigned int iterations=100);

    /*!
     * \brief Compute the optimal line search step along the input direction,
     * moving as far as possible while satisfying the Wolfe conditions.  Note
     * that this is the minimizing step.
     *
     * \param f The function to be minimized.
     * \param f_grad Gradient function for the function to be minimized.
     * \param x State to conduct line search from
     * \param direction Direction for line search
     * \param step_1 Initial step to consider
     * \param c_1 Sufficient decrease parameter
     * \param c_2 Curvature parameter
     * \param iterations Maximum number of iterations.
     */
    // Returns optimal step. Note that this is the minimizing step, not the maximizing.
    double wolfe_condition_line_search(RealFunc f, MultiFunc f_grad, const
        VectorXd& x, const VectorXd& direction, double step_1 = 1.0, double c_1 = 1e-4, 
        double c_2 = 0.5, unsigned int iterations=100);

  } // namespace ml
} // namespace cannon

#endif /* ifndef CANNON_ML_WOLFE_CONDITIONS_H */
