#ifndef CANNON_MATH_POLYNOMIAL_H
#define CANNON_MATH_POLYNOMIAL_H 

/*!
 * \file cannon/math/polynomial.hpp
 * \brief File containing Polynomial class definition and utilities.
 */

#include <vector>

namespace cannon {
  namespace math {

    /*!
     * \brief Class representing a polynomial in a certain basis.
     */
    template <typename Basis>
    class Polynomial {
      public:
        Polynomial() = delete;

        /*!
         * \brief Constructor taking number of coefficients for this polynomial.
         */
        Polynomial(unsigned int dim) : coeffs_(dim, 0) {}

        /*!
         * \brief Constructor taking a vector of coefficients.
         */
        Polynomial(const std::vector<double> coeffs) : coeffs_(coeffs) {}

        /*!
         * \brief Copy constructor.
         */
        Polynomial(Polynomial& o) : coeffs_(o.coeffs_) {}

        /*!
         * \brief Move constructor.
         */
        Polynomial(Polynomial&& o) : coeffs_(o.coeffs_) {
          o.coeffs_.clear();
        }

        /*!
         * \brief Get the coefficients of this Polynomial.
         *
         * \returns A vector of coefficients for the Polynomial in its basis.
         */
        const std::vector<double>& coefficients() {
          return coeffs_;
        }

        /*!
         * \brief Evaluation operator.
         */
        double operator()(double x) {
          double ret_sum = 0;
          for (unsigned int i = 0; i < coeffs_.size(); ++i) {
            ret_sum += coeffs_[i] * Basis::evaluate(i, x);
          }

          return ret_sum;
        }

        /*!
         * \brief Compound addition operator.
         */
        Polynomial& operator+=(const Polynomial& o) {
          for (unsigned int i = 0; i < std::min(coeffs_.size(), o.coeffs_.size()); ++i)
            coeffs_[i] += o.coeffs_[i];

          if (o.coeffs_.size() > coeffs_.size()) {
            auto old_size = coeffs_.size();
            coeffs_.resize(o.coeffs_.size());

            for (unsigned int i = old_size; i < coeffs_.size(); ++i)
              coeffs_[i] = o.coeffs_[i];
          }

          return *this;
        }

        /*!
         * \brief Friend addition operator.
         */
        friend Polynomial operator+(Polynomial lhs, const Polynomial& rhs) {
          lhs += rhs;
          return lhs;
        }

        /*!
         * \brief Compound scalar multiplication operator.
         */
        Polynomial& operator*=(double a) {
          for (unsigned int i = 0; i < coeffs_.size(); ++i)
            coeffs_[i] *= a;

          return *this;
        }

        /*!
         * \brief Friend scalar multiplication operator.
         */
        friend Polynomial operator*(Polynomial lhs, double a) {
          lhs *= a;
          return lhs;
        }

      private:
        std::vector<double> coeffs_; //!< Coefficients of this polynomial in its basis
    };

    /*!
     * \brief Class satisfying the requirements of a polynomial basis, i.e.,
     * providing an static evaluate function. Represents the common monomial
     * basis, in which each basis element is of the form x^n.
     */
    class MonomialBasis {
      public:
        static double evaluate(unsigned int idx, double x);
    };


  } // namespace math
} // namespace cannon

#endif /* ifndef CANNON_MATH_POLYNOMIAL_H */
