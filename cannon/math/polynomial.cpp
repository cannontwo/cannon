#include <cannon/math/polynomial.hpp>

#include <cmath>

using namespace cannon::math;

double MonomialBasis::evaluate(unsigned idx, double x) {
  return std::pow(x, idx);
}
