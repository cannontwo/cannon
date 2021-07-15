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
#include <set>

#include <cannon/log/registry.hpp>

using namespace cannon::log;

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
      if (upper < 2) {
        std::vector<T> empty;
        return empty;
      }

      // 0 and 1 are not primes
      static std::vector<bool> sieve(2, false);

      if (sieve.size() < upper + 1) {
        sieve.resize(upper+1, true);
        
        for (T i = 0; i <= std::sqrt(upper); ++i) {
          if (sieve[i]) {
            for (T j = i*i; j <= upper; j += i) {
              sieve[j] = false;
            }
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
     * \brief Check if the input number is prime. Note this is more efficient
     * than simply running the Sieve of Eratosthenes and checking for
     * inclusion.
     *
     * \param x The number to check for primeness.
     *
     * \returns Whether @param x is prime
     */
    template <typename T>
    typename std::enable_if<std::is_integral<T>::value, bool>::type
    is_prime(T x) {
      if (x == 1 || x == 0)
        return false;

      for (T i = 2; i <= std::sqrt(x); ++i) {
        if ((x % i) == 0)
          return false;
      }
      
      return true;
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

    /*!
     * \brief Get prime factorization of the input number.
     *
     * \param n The number to factorize.
     *
     * \returns A multiset of prime factors, where the number of entries for a
     * particular prime is the number of times that the prime occurs in the
     * prime factorization.
     */
    template <typename T>
    typename std::enable_if<std::is_integral<T>::value, std::multiset<T>>::type
    get_prime_factorization(T n) {
      std::multiset<T> factors;
      std::vector<T> primes =
          get_primes_up_to(static_cast<T>(std::floor(std::sqrt(n))));

      T tmp = n;

      for (auto p : primes) {
        while (tmp % p == 0) {
          factors.insert(p);
          tmp /= p;
        }
      }

      if (tmp != 1)
        factors.insert(tmp);

      return factors;
    }

  } // namespace math
} // namespace cannon

#endif /* ifndef CANNON_MATH_PRIMES_H */
