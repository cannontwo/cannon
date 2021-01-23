#include <cannon/ml/optimization_result.hpp>

using namespace cannon::ml;

std::ostream& cannon::ml::operator<<(std::ostream& os, OptimizationResult result) {
  os << "Optimization Result:" << std::endl;
  os << "\tObjective: " << result.objective << std::endl;
  os << "\tSolution: " << result.solution << std::endl;
  os << "\tIterations: " << result.iterations << std::endl;
  os << "\tHit Max Iters: " << result.max_iterations << std::endl;

  return os;
}
