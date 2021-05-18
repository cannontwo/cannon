#ifndef CANNON_ML_BFGS_H
#define CANNON_ML_BFGS_H 

#include <functional>

#include <Eigen/Dense>

#include <cannon/utils/class_forward.hpp>

using namespace Eigen;

using RealFunc = std::function<double(const VectorXd&)>;
using MultiFunc = std::function<VectorXd(const VectorXd&)>;

namespace cannon {
  namespace ml {

    CANNON_CLASS_FORWARD(OptimizationResult);

    class BFGSOptimizer {
      public:
        BFGSOptimizer() = delete;

        BFGSOptimizer(RealFunc f, MultiFunc f_grad) : f_(f), f_grad_(f_grad) {}

        OptimizationResult optimize(const VectorXd& start, double eps=1e-4, unsigned
            int iterations=100);

      private:
        RealFunc f_;
        MultiFunc f_grad_;

    };

  } // namespace ml
} // namespace cannon

#endif /* ifndef CANNON_ML_BFGS_H */
