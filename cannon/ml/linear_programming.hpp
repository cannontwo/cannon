#ifndef CANNON_ML_LINEAR_PROGRAMMING_H
#define CANNON_ML_LINEAR_PROGRAMMING_H 

/*!
 * \file cannon/ml/linear_programming.hpp
 * \brief File containing LPOptimizer class definition.
 */

#ifdef CANNON_HAVE_GUROBI
#include <memory>

#include <gurobi_c++.h>
#include <Eigen/Dense>

#include <cannon/utils/class_forward.hpp>

using namespace Eigen;

namespace cannon {
  namespace ml {

    CANNON_CLASS_FORWARD(OptimizationResult);

    /*!
     * \brief Class representing a linear programming problem. Solving this
     * optimization problem is done using Gurobi.
     */
    class LPOptimizer {
      public:
        LPOptimizer() = delete;

        /*!
         * \brief Constructor taking variable constraints and the linear
         * objective function cooeficients for a linear programming problem.
         * The number of variables in the linear program is implicitly defined
         * by the size of these vectors.
         */
        LPOptimizer(const VectorXd& lower, const VectorXd& upper, const
            VectorXd& objective_vec) : env_(true), num_vars_(lower.size()) {
          assert(lower.size() == upper.size());
          assert(lower.size() == objective_vec.size());

          env_.start();
          model_ = std::make_shared<GRBModel>(env_);

          // Improve numeric issues
          //model_->set(GRB_IntParam_NumericFocus, 3);
          //model_->set(GRB_IntParam_Presolve, 0);

          // Since this is just an LP, we don't need crossover (I think).
          // We can only disable this by explicitly using the barrier method. Note that we
          // are fine with this because approximately optimal solutions
          // satisfying the constraints are usually all we need.
          //model_->set(GRB_IntParam_Method, 2);
          //model_->set(GRB_IntParam_BarHomogeneous, 1);
          //model_->set(GRB_DoubleParam_BarConvTol, 1e-1);
          //model_->set(GRB_IntParam_Crossover, 0);

          // Create Gurobi variables
          vars_ = model_->addVars(lower.data(), upper.data(), NULL, NULL, NULL, num_vars_);

          // Create Gurobi linear objective
          GRBLinExpr obj = 0;
          for (unsigned int i = 0; i < num_vars_; i++) {
            obj += vars_[i] * objective_vec[i];
          }
          model_->setObjective(obj, GRB_MINIMIZE);
        }

        /*!
         * \brief Destructor.
         */
        ~LPOptimizer() {
          // vars_ is allocated on the heap and has to be freed
          delete[] vars_;
        }

        /*!
         * \brief Add a constraint of the form A * x <= b to this linear
         * programming problem. 
         *
         * \param lhs_mat Matrix multiplying the variables of this problem in the new constraint.
         * \param rhs Vector on the right hand side of the inequality for the new constraint.
         */
        void add_constraint(const MatrixXd& lhs_mat, const VectorXd& rhs);

        /*!
         * \brief Minimize the linear program represented by this object.
         * Throws std::runtime_error and GRBException, as well as misc other
         * exceptions.
         *
         * \returns The result of attempting to solve the linear program.
         */
        OptimizationResult optimize();
        

      private:
        GRBEnv env_; //!< Gurobi environment
        std::shared_ptr<GRBModel> model_; //!< Gurobi model representing this LP

        GRBVar* vars_; //!< Gurobi variables for this problem
        unsigned int num_vars_; //!< Number of variables in this problem

    };

  } // namespace ml
} // namespace cannon

#endif  // CANNON_HAVE_GUROBI

#endif /* ifndef CANNON_ML_LINEAR_PROGRAMMING_H */
