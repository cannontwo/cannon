#ifdef CANNON_HAVE_GUROBI

#include <cannon/ml/linear_programming.hpp>

#include <cannon/ml/optimization_result.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::ml;
using namespace cannon::log;

void LPOptimizer::add_constraint(const MatrixXd& lhs_mat, const VectorXd& rhs) {
  assert(lhs_mat.cols() == num_vars_);
  assert(lhs_mat.rows() == rhs.size());

  for (unsigned int i = 0; i < rhs.size(); i++) {
    GRBLinExpr lhs = 0;

    for (unsigned int j = 0; j < num_vars_; j++) {
      lhs += lhs_mat(i, j) * vars_[j];
    }

    model_->addConstr(lhs, GRB_LESS_EQUAL, rhs[i]);
  }
}

OptimizationResult LPOptimizer::optimize() {
  model_->optimize();

  if (model_->get(GRB_IntAttr_Status) == GRB_OPTIMAL) {
    log_info("Gurobi found optimal solution");
    VectorXd solution = VectorXd::Zero(num_vars_);

    double objective = model_->get(GRB_DoubleAttr_ObjVal);
    for (unsigned int i = 0; i < num_vars_; i++) {
      solution[i] = vars_[i].get(GRB_DoubleAttr_X);
    }

    return OptimizationResult(objective, solution, 0, false);
  } else if (model_->get(GRB_IntAttr_Status) == GRB_SUBOPTIMAL) {
    log_info("Gurobi found suboptimal solution.");

    VectorXd solution = VectorXd::Zero(num_vars_);

    double objective = model_->get(GRB_DoubleAttr_ObjVal);
    for (unsigned int i = 0; i < num_vars_; i++) {
      solution[i] = vars_[i].get(GRB_DoubleAttr_X);
    }

    return OptimizationResult(objective, solution, 0, false);
  } else {
    throw std::runtime_error("Gurobi could not find solution to LP");
  }
}
#endif
