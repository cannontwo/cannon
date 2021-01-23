#include <catch2/catch.hpp>

#include <cannon/ml/wolfe_conditions.hpp>
#include <cannon/math/func/rosenbrock.hpp>

using namespace cannon::ml;
using namespace cannon::math;

double f(const Vector2d& x) {
  return x.dot(x);
}

Vector2d f_grad(const Vector2d& x) {
  return 2.0 * x;
}

TEST_CASE("WolfeConditions", "[ml]") {
  Vector2d x = Vector2d::Zero();
  x[0] = 4.0;
  x[1] = 4.0;

  for (unsigned int i = 0; i < 10; i++) {
    Vector2d p = -0.1 * f_grad(x);
    log_info("f(x) = ", f(x), ", p is", p);
    double step = wolfe_condition_line_search(f, f_grad, x, p);
    log_info("Line search produced step", step);
    log_info("f(x + step*p) = ", f(x + step*p), "\n");

    x = x + step*p;
  }

  REQUIRE(f(x) < 1e-4);

  x = Vector2d::Zero();
  for (unsigned int i = 0; i < 2000; i++) {
    Vector2d p = -rosenbrock_grad(x);
    log_info("rosenbrock(x) = ", rosenbrock(x), ", p is", p);
    double step = wolfe_condition_line_search(rosenbrock, rosenbrock_grad, x, p);
    log_info("Line search produced step", step);
    log_info("rosenbrock(x + step*p) = ", rosenbrock(x + step*p), "\n");

    x = x + step*p;
    log_info("x is now", x);
  }

  REQUIRE(rosenbrock(x) < 1e-4);

}
