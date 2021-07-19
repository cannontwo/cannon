#include <cannon/math/polynomial.hpp>

#include <cmath>

using namespace cannon::math;

// Monomial Basis

double MonomialBasis::evaluate(unsigned idx, double x) {
  return std::pow(x, idx);
}

Polynomial<MonomialBasis>
MonomialBasis::multiply(const Polynomial<MonomialBasis> &lhs,
                        const Polynomial<MonomialBasis> &rhs) {
  std::vector<double> coeffs(
      lhs.coefficients().size() + rhs.coefficients().size(), 0);

  for (unsigned int i = 0; i < lhs.coefficients().size(); ++i) {
    for (unsigned int j = 0; j < rhs.coefficients().size(); ++j) {
      coeffs[i + j] += lhs.coefficients()[i] * rhs.coefficients()[j];
    }
  }

  return Polynomial<MonomialBasis>(coeffs);
}

void MonomialBasis::print(std::ostream& os, const Polynomial<MonomialBasis>& p) {
  bool printed = false;

  for (unsigned int i = 0; i < p.coefficients().size(); ++i) {
    unsigned int pow = p.coefficients().size() - 1 - i;

    if (p.coefficients()[pow] != 0) {
      if (printed)
        os << " + ";
      else
        printed = true;

      if (p.coefficients()[pow] != 1 || pow == 0)
        os << p.coefficients()[pow];

      if (pow != 0)
        os << "x^" << pow;
    }
  }
}

Polynomial<MonomialBasis> MonomialBasis::x =
    Polynomial<MonomialBasis>{std::vector<double>{0, 1}};
