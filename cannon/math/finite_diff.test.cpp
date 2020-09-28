#include <iostream>
#include <cmath>
#include <cannon/math/finite_diff.hpp>

double sin_plus_cos(double x) {
  return sin(x) + cos(x);
}

int main(void)
{
  std::cout << "Testing finite_diff" << std::endl; 
  std::cout << fin_diff(sin_plus_cos, 1., 0.001) << std::endl;
  std::cout << fin_diff(sin_plus_cos, 0., 0.001) << std::endl;

  return 0;
}
