#include <iostream>
#include <cmath>
#include <cassert>

#include <cannon/math/finite_diff_derivative.hpp>
#include <cannon/math/nearly_equal.hpp>

using namespace cannon::math; 

double sin_plus_cos(double x) {
  return sin(x) + cos(x);
}

struct sc_f {
  double operator() (double x) const {
    return sin(x) + cos(x);
  }
};

class psc_f {
  public:
    psc_f(double alpha) : alpha(alpha) {}

    double operator() (double x) const {
      return sin(alpha * x) + cos(x);
    }

  private:
    double alpha;
};

double linear(double x) {
  return x;
}

double quadratic(double x) {
  return x * x;
}

int main(void)
{
  psc_f psc_o(1.0);
  auto d_psc_o = make_nth_derivative<1>(psc_o, 0.001);

  std::cout << "Testing finite_diff_derivative" << std::endl; 
  std::cout << d_psc_o(1.0) << std::endl;
  std::cout << make_nth_derivative<1>(psc_f(2.0), 0.001)(1.0) << std::endl;
  std::cout << make_nth_derivative<1>(sin_plus_cos, 0.001)(0.0) << std::endl;

  assert(nearly_equal(make_nth_derivative<1>(linear, 0.001)(1.0),1.0));
  assert(nearly_equal(make_nth_derivative<1>(linear, 0.001)(2.0),1.0));

  assert(nearly_equal(make_nth_derivative<1>(quadratic, 0.00001)(1.0),2.0));
  assert(nearly_equal(make_nth_derivative<1>(quadratic, 0.00001)(-1.0),-2.0));

  return 0;
}
