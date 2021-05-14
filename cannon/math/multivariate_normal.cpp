#include <cannon/math/multivariate_normal.hpp>

using namespace cannon::math;

std::random_device MultivariateNormal::rd;
std::mt19937 MultivariateNormal::gen(MultivariateNormal::rd());
std::normal_distribution<double> MultivariateNormal::dist;

void MultivariateNormal::set_seed(int seed) {
  MultivariateNormal::gen = std::mt19937(seed);  
}
