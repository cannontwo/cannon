#ifndef CANNON_FINITE_DIFF_DERIVATIVE_H
#define CANNON_FINITE_DIFF_DERIVATIVE_H 

/*!
 * \file cannon/math/finite_diff_derivative.hpp
 * \brief File containing finite difference derivative tools for real-valued
 * functions.
 */

namespace cannon {
  namespace math {

    /*!
     * \brief Functor representing the Nth derivative of a function. Note that
     * each level of the derivative is computed by single-order finite
     * difference, so error accumulates quickly.
     */
    template <typename F, typename T, unsigned N>
    class nth_derivative {
      using prev_derivative = nth_derivative<F, T, N-1>;

      public:
        
        /*!
         * \brief Cosntructor taking a function to take derivatives of and
         * amount to add for finite differencing.
         */
        nth_derivative(const F& f, const T& h) : h(h), fp(f,h) {}

        /*!
         * \brief Return the value of the Nth derivative of this object's
         * function.
         *
         * \param x Point in input space to compute Nth derivative for.
         */
        T operator()(const T& x) const {
          return (fp(x+h) - fp(x)) / h;
        }

      private:
        T h; //!< Amount to add for finite differencing
        prev_derivative fp; //!< Recursive N-1th derivative
    };

    /*!
     * \brief Functor representing the base case of recursive finite difference
     * derivatives.
     */
    template <typename F, typename T>
    class nth_derivative<F, T, 1> {
      public:

        /*!
         * \brief Cosntructor taking a function to take derivatives of and
         * amount to add for finite differencing.
         */
        nth_derivative(const F& f, const T& h): f(f), h(h) {}

        /*!
         * \brief Return the value of the Nth derivative of this object's
         * function.
         *
         * \param x Point in input space to compute Nth derivative for.
         */
        T operator()(const T& x) const {
          return (f(x+h) - f(x)) / h;
        }

      private:
        const F& f; //!< Function to take derivatives of
        T h; //!< Amount to add for finite differencing
    };

    /*!
     * \brief Utility function to create Nth derivative for an input real-valued function.
     *
     * \param f The function to take a derivative of.
     * \param h Amount to add for finite differencing
     *
     * \returns Object representing the Nth derivative of the input function.
     */
    template<unsigned N, typename F, typename T>
    nth_derivative<F, T, N> make_nth_derivative(const F& f, const T& h) {
      return nth_derivative<F, T, N>(f, h);
    }

  } // namespace math
} // namespace cannon

#endif /* ifndef CANNON_FINITE_DIFF_DERIVATIVE_H */

