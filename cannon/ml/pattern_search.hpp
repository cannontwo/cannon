#ifndef CANNON_ML_PATTERN_SEARCH_H
#define CANNON_ML_PATTERN_SEARCH_H 

/*!
 * \file cannon/ml/pattern_search.hpp
 * \brief File containing PatternSearchOptimizer class definition.
 */

#include <functional>

#include <Eigen/Dense>

#include <cannon/utils/class_forward.hpp>

using namespace Eigen;

using RealFunc = std::function<double(const VectorXd&)>;

namespace cannon {
  namespace ml {

    CANNON_CLASS_FORWARD(OptimizationResult);

    /*!
     * \brief Class representing a pattern search optimizer. Pattern Search is
     * a derivative-free optimization method, similar to the Nelder-Mead method
     * but with better convergence guarantees. See
     * https://en.wikipedia.org/wiki/Pattern_search_(optimization).
     */ 
    class PatternSearchOptimizer {
      public:
        PatternSearchOptimizer() = delete;

        /*!
         * \brief Constructor taking a function to optimize, the dimension of
         * input space, whether to reorient the search pattern, and tuning
         * parameters.
         */
        PatternSearchOptimizer(RealFunc f, unsigned int dim, bool
            reorient=false, double forcing_func_param=1.0, double reorient_param=0.1) : f_(f), dim_(dim),
        reorient_(reorient), forcing_func_param_(forcing_func_param), reorient_param_(reorient_param) {}

        /*!
         * \brief Minimize the unconstrained, derivative-free problem represented by this object. 
         *
         * \param start Start state for iterative optimization
         * \param iterations Maximum number of iterations to perform
         * \param eps Small number used for testing convergence
         * \param contraction_param Parameter used to shrink search step size
         * \param expansion_param Parameter used to expand search step size
         * \param initial_step Initial step size
         */ 
        OptimizationResult optimize(const VectorXd& start, unsigned int iterations=1000, double eps=1e-10,
            double contraction_param=0.9, double expansion_param=1.5, double
            initial_step=1.0);

      private:

        /*!
         * \brief Create a positive spanning simplex set from the input vector.
         * This ensures that the input direction is a search direction. 
         *
         * \param first The vector to build a simplex set around.
         *
         * \returns The simplex direction set.
         */
        std::vector<VectorXd> create_simplex_set_(const VectorXd& first);

        /*!
         * \brief Forcing function used to ensure that function evaluations
         * decrease sufficiently.
         *
         * \param t Input used to compute forcing term.
         *
         * \returns Forcing term.
         */
        double forcing_func_(double t);

        /*!
         * \brief Update leading direction by exponential averaging with the
         * input difference vector.
         *
         * \param leading_dir The leading search direction to update.
         * \param diff Difference vector to use for updating.
         */
        void update_leading_dir_(VectorXd& leading_dir, const VectorXd& diff);

        RealFunc f_; //!< Function to be optimized
        unsigned int dim_; //!< Input dimension
        bool reorient_; //!< Whether to reorient leading direction 

        double forcing_func_param_; //!< Forcing function parameter
        double reorient_param_; //!< Reorientation speed parameter

    };

  } // namespace ml
} // namespace cannon

#endif /* ifndef CANNON_ML_PATTERN_SEARCH_H */
