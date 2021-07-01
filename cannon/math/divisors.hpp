#pragma once
#ifndef CANNON_MATH_DIVISORS_H
#define CANNON_MATH_DIVISORS_H 

#include <type_traits>

#include <cannon/math/primes.hpp>

/*!
 * \file cannon/math/divisors.hpp
 * \brief File containing utilities for computing divisors of integers.
 */

namespace cannon {
  namespace math {

    template <typename T>
    typename std::enable_if<std::is_integral<T>::value, T>::type
    get_divisor_sum(T x) {
      if (x == 0)
        return 0;

      auto prime_factors = get_prime_factorization(x);

      unsigned int total_sum = 1;
      for (auto it = prime_factors.begin(); it != prime_factors.end();) {
        T factor = *it;
        T prime_power_sum = 0;

        // See https://www2.math.upenn.edu/~deturck/m170/wk3/lecture/sumdiv.html
        for (unsigned int i = 0; i < prime_factors.count(factor) + 1; ++i) {
          prime_power_sum += std::pow(factor, i);
        }

        total_sum *= prime_power_sum;

        do {
          ++it;
        } while (it != prime_factors.end() && *it == factor);
      }

      return total_sum - x;
    }
  }
}

#endif /* ifndef CANNON_MATH_DIVISORS_H */
