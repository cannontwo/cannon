#ifndef CANNON_ML_LINEAR_PROGRAMMING_H
#define CANNON_ML_LINEAR_PROGRAMMING_H 

#include <memory>

#include <gurobi_c++.h>
#include <Eigen/Dense>

#include <cannon/ml/optimization_result.hpp>

using namespace Eigen;

namespace cannon {
  namespace ml {

    // TODO Wrap around Gurobi functionality
    
    class LPOptimizer {
      public:
        LPOptimizer() = delete;

        LPOptimizer(const VectorXd& lower, const VectorXd& upper, const
            VectorXd& objective_vec) : env_(true), num_vars_(lower.size()) {
          assert(lower.size() == upper.size());
          assert(lower.size() == objective_vec.size());

          env_.start();
          model_ = std::make_shared<GRBModel>(env_);

          // Create Gurobi variables
          vars_ = model_->addVars(lower.data(), upper.data(), NULL, NULL, NULL, num_vars_);

          // Create Gurobi linear objective
          GRBLinExpr obj = 0;
          for (unsigned int i = 0; i < num_vars_; i++) {
            obj += vars_[i] * objective_vec[i];
          }
          model_->setObjective(obj, GRB_MINIMIZE);
        }

        ~LPOptimizer() {
          // vars_ is allocated on the heap and has to be freed
          delete[] vars_;
        }

        // Constraints are assumed to be of the form A * x <= b
        void add_constraint(const MatrixXd& lhs_mat, const VectorXd& rhs);

        // Minimizes by default. Throws std::runtime_error and GRBException, as
        // well as misc other exceptions
        OptimizationResult optimize();
        

      private:
        GRBEnv env_;
        std::shared_ptr<GRBModel> model_;

        GRBVar* vars_;
        unsigned int num_vars_;

    };

  } // namespace ml
} // namespace cannon

#endif /* ifndef CANNON_ML_LINEAR_PROGRAMMING_H */
