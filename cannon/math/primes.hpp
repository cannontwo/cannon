#pragma once
#ifndef CANNON_MATH_PRIMES_H
#define CANNON_MATH_PRIMES_H 

/*!
 * \file cannon/math/primes.hpp
 * \brief File containing utilities for working with prime numbers.
 */

#include <type_traits>
#include <vector>
#include <cmath>

namespace cannon {
  namespace math {

  
    /*!
     * \brief Get all primes up to and including the input number. Uses the
     * Sieve of Eratosthenes
     * (https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes)
     *
     * Templated to work for all integral types.
     *
     * \param upper Upper limit for prime finding.
     *
     * \returns A vector of the primes up to upper, sorted in increasing order.
     */
    template <typename T>
    typename std::enable_if<std::is_integral<T>::value, std::vector<T>>::type
    get_primes_up_to(T upper) {

      std::vector<bool> sieve(upper+1, true);
      sieve[0] = false;
      sieve[1] = false;

      for (T i = 0; i <= std::sqrt(upper); ++i) {
        if (sieve[i]) {
          for (T j = i*i; j <= upper; j += i) {
            sieve[j] = false;
          }
        }
      }

      std::vector<T> primes;
      for (T i = 0; i <= upper; ++i) {
        if (sieve[i])
          primes.push_back(i);
      }
      
      return primes;
    }

    /*!
     * \brief Get the largest prime factor of the input number. Returns the
     * number itself if it's prime.
     *
     * \param n The number to get the largest prime factor of.
     *
     * \returns The largest prime factor.
     */
    template <typename T>
    typename std::enable_if<std::is_integral<T>::value, T>::type
    get_largest_prime_factor(T n) {
      std::vector<T> primes =
          get_primes_up_to(static_cast<T>(std::floor(std::sqrt(n))));
      T largest_prime_factor = 0;

      for (std::size_t i = 0; i < primes.size(); ++i) {
        if (n % primes[i] == 0) 
          largest_prime_factor = primes[i];
      }

      if (largest_prime_factor == 0)
        return n;
      else
        return largest_prime_factor;
    
    }

  } // namespace math
} // namespace cannon

#endif /* ifndef CANNON_MATH_PRIMES_H */
