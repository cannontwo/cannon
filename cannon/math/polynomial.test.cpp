#include <catch2/catch.hpp>

#include <cannon/log/registry.hpp>
#include <cannon/math/polynomial.hpp>
#include <cannon/math/nearly_equal.hpp>

using namespace cannon::math;
using namespace cannon::log;

TEST_CASE("Polynomial", "[math]") {
  Polynomial<MonomialBasis> p(0);
  REQUIRE(p.coefficients().size() == 0);

  log_info(p);

  for (unsigned int i = 0; i < 10; ++i)
    REQUIRE(p(0.1*i) == 0);

  std::vector<double> coeffs;
  coeffs.push_back(1.0);
  Polynomial<MonomialBasis> p2(coeffs);

  p += p2;

  REQUIRE(p.coefficients().size() == 1);
  REQUIRE(p.coefficients()[0] == 1.0);

  log_info(p);

  for (unsigned int i = 0; i < 10; ++i)
    REQUIRE(p(0.1*i) == 1);

  p *= 2;
  REQUIRE(p.coefficients()[0] == 2.0);

  log_info(p);

  for (unsigned int i = 0; i < 10; ++i)
    REQUIRE(p(0.1*i) == 2);

  coeffs[0] = 0.0;
  coeffs.push_back(1.0);
  Polynomial<MonomialBasis> x(coeffs);
  p += x;

  log_info(p);

  REQUIRE(p.coefficients().size() == 2);
  REQUIRE(p.coefficients()[0] == 2);
  REQUIRE(p.coefficients()[1] == 1);
  for (unsigned int i = 0; i < 10; ++i)
    REQUIRE(p(0.1*i) == 2 + 0.1*i);

  coeffs[1] = 0.0;
  coeffs.push_back(1.0);
  Polynomial<MonomialBasis> x_sqr(coeffs);
  p += x_sqr;

  log_info(p);

  REQUIRE(p.coefficients().size() == 3);
  for (unsigned int i = 0; i < 10; ++i)
    REQUIRE(p(0.1*i) == 2 + 0.1*i + 0.01*i*i);

  Polynomial<MonomialBasis> p3;
  p3 += 1;
  p3 *= MonomialBasis::x;
  
  log_info(p3);

  for (unsigned int i = 0; i < 10; ++i)
    REQUIRE(p3(0.1*i) == 0.1*i);

  p3 *= p3;
  log_info(p3);
  for (unsigned int i = 0; i < 10; ++i)
    REQUIRE(nearly_equal(p3(0.1*i), 0.01*i*i));

  p3 += 1;
  log_info(p3);
  p3 *= p;
  log_info(p3);
}
