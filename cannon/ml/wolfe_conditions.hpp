#ifndef CANNON_ML_WOLFE_CONDITIONS_H
#define CANNON_ML_WOLFE_CONDITIONS_H 

#include <functional>

#include <Eigen/Dense>

using namespace Eigen;

using RealFunc = std::function<double(const VectorXd&)>;
using MultiFunc = std::function<VectorXd(const VectorXd&)>;

namespace cannon {
  namespace ml {

    bool sufficient_decrease_condition(RealFunc f, MultiFunc f_grad, const
        VectorXd& x, double step, const VectorXd& direction, double c_1);

    // *Strong* Wolfe conditions
    bool curvature_condition(RealFunc f, MultiFunc f_grad, const
        VectorXd& x, double step, const VectorXd& direction, double c_2);

    double wolfe_conditions(RealFunc f, MultiFunc f_grad, const
        VectorXd& x, double step, const VectorXd& direction, double c_1, 
        double c_2);

    // Returns optimal step. Note that this is the minimizing step, not the maximizing.
    double line_search_zoom(RealFunc f, MultiFunc f_grad, const VectorXd& x,
        const VectorXd& direction, double start_low, double start_high,
        double c_1, double c_2, unsigned int iterations=100);

    // Returns optimal step. Note that this is the minimizing step, not the maximizing.
    double wolfe_condition_line_search(RealFunc f, MultiFunc f_grad, const
        VectorXd& x, const VectorXd& direction, double step_1 = 1.0, double c_1 = 1e-4, 
        double c_2 = 0.5, unsigned int iterations=100);

  } // namespace ml
} // namespace cannon

#endif /* ifndef CANNON_ML_WOLFE_CONDITIONS_H */
