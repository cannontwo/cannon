#ifndef CANNON_MATH_POLYNOMIAL_H
#define CANNON_MATH_POLYNOMIAL_H 

/*!
 * \file cannon/math/polynomial.hpp
 * \brief File containing Polynomial class definition and utilities.
 */

#include <vector>
#include <iostream>

namespace cannon {
  namespace math {

    /*!
     * \brief Class representing a polynomial in a certain basis.
     */
    template <typename Basis>
    class Polynomial {
      public:
        Polynomial() : Polynomial(0) {}

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
        Polynomial(const Polynomial& o) : coeffs_(o.coeffs_) {}

        /*!
         * \brief Copy assignment operator.
         */
        Polynomial& operator=(const Polynomial& o) {
          coeffs_ = o.coeffs_; 
          return *this;
        }

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
        const std::vector<double>& coefficients() const {
          return coeffs_;
        }

        /*!
         * \brief Evaluation operator.
         */
        double operator()(double x) const {
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
         * \brief Compound scalar addition operator.
         */
        Polynomial& operator+=(double x) {
          if (coeffs_.size() == 0)
            coeffs_.resize(1, 0);

          coeffs_[0] += x;

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
         * \brief Friend addition operator.
         */
        friend Polynomial operator+(Polynomial lhs, double x) {
          lhs += x;
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
         * \brief Compound multiplication operator.
         */
        Polynomial& operator*=(const Polynomial& o) {
          // Different bases may have different, more efficient ways of doing
          // multiplication. A default (inefficient) way would be to convert to
          // MonomialBasis and use that multiplication.
          *this = *this * o;
          return *this;
        }

        /*!
         * \brief Friend scalar multiplication operator.
         */
        friend Polynomial operator*(Polynomial lhs, double a) {
          lhs *= a;
          return lhs;
        }

        /*!
         * \brief Friend multiplication operator.
         */
        friend Polynomial operator*(Polynomial lhs, const Polynomial& rhs) {
          return Basis::multiply(lhs, rhs);
        }

        friend std::ostream& operator<<(std::ostream& os, const Polynomial& p) {
          Basis::print(os, p);
          return os;
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

        /*!
         * \brief Evaluate a monomial of the idx power at the point x.
         *
         * \param idx Power of monomial to evaluate.
         * \param x Input point.
         *
         * \returns Monomial value at x.
         */
        static double evaluate(unsigned int idx, double x);

        /*!
         * \brief Multiply two polynomials over the monomial basis.
         *
         * \param lhs First polynomial to multiply.
         * \param rhs The other polynomial to multiply.
         */
        static Polynomial<MonomialBasis>
        multiply(const Polynomial<MonomialBasis> &lhs,
                 const Polynomial<MonomialBasis> &rhs);

        /*!
         * \brief Print the input polynomial to the input stream.
         *
         * \param os The stream to print to.
         * \param p The polynomial to print.
         */
        static void print(std::ostream& os, const Polynomial<MonomialBasis>& p);

        static Polynomial<MonomialBasis> x; //!< Basic monomial
    };


  } // namespace math
} // namespace cannon

#endif /* ifndef CANNON_MATH_POLYNOMIAL_H */
