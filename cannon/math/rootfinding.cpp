#include <cannon/math/rootfinding.hpp>

#include <cassert>
#include <cmath>

#include <cannon/math/nearly_equal.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::math;
using namespace cannon::log;

double cannon::math::bisection_method(std::function<double(double)> f, double a,
                                      double b, unsigned int iters) {
  assert(a < b);
  assert(f(a) * f(b) < 0);

  for (unsigned int i = 0; i < iters; ++i) {
    double c = 0.5 * (a + b);

    if (nearly_equal(f(c), 0.0))
      return c;

    if (f(a) * f(c) < 0) {
      b = c;
    } else {
      a = c;
    }
  }

  return 0.5 * (a + b);
}
