#ifndef CANNON_ML_PATTERN_SEARCH_H
#define CANNON_ML_PATTERN_SEARCH_H 

#include <functional>

#include <Eigen/Dense>

#include <cannon/ml/optimization_result.hpp>
#include <cannon/log/registry.hpp>

using namespace Eigen;

using namespace cannon::log;

using RealFunc = std::function<double(const VectorXd&)>;

namespace cannon {
  namespace ml {

    class PatternSearchOptimizer {
      public:
        PatternSearchOptimizer() = delete;

        PatternSearchOptimizer(RealFunc f, unsigned int dim, bool
            reorient=false, double forcing_func_param=1.0, double reorient_param=0.1) : f_(f), dim_(dim),
        reorient_(reorient), forcing_func_param_(forcing_func_param), reorient_param_(reorient_param) {}

        OptimizationResult optimize(const VectorXd& start, unsigned int iterations=1000, double eps=1e-10,
            double contraction_param=0.9, double expansion_param=1.5, double
            initial_step=1.0);

      private:
        std::vector<VectorXd> create_simplex_set_(const VectorXd& first);
        double forcing_func_(double t);
        void update_leading_dir_(VectorXd& leading_dir, const VectorXd& diff);

        RealFunc f_;
        unsigned int dim_;
        bool reorient_;

        double forcing_func_param_;

        double reorient_param_;

    };

  } // namespace ml
} // namespace cannon

#endif /* ifndef CANNON_ML_PATTERN_SEARCH_H */
