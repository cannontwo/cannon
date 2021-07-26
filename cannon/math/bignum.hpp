#pragma once
#ifndef CANNON_MATH_BIGNUM_H
#define CANNON_MATH_BIGNUM_H

/*!
 * \file cannon/math/bignum.hpp
 * \brief File containing classes and utilities for manipulating
 * arbitrary-precision numbers.
 */

#include <string>
#include <vector>
#include <iostream>

namespace cannon {
  namespace math {

    /*!
     * \brief Class representing an arbitrary-size unsigned integer. 
     *
     * TODO Switch to using GMP at some point.
     */
    class BigUnsigned {
      public:

        /*!
         * \brief Default constructor
         */
        BigUnsigned() {
          digits_.push_back(0);
        }

        /*!
         * \brief Constructor taking an unsigned int to initialize this object
         * with.
         */
        BigUnsigned(unsigned int x);

        /*!
         * \brief Constructor taking a string representing the number to
         * initialize.
         */
        BigUnsigned(const std::string& num);

        /*!
         * \brief Constructor taking a vector of digits representing the number
         * to initialize.
         */
        BigUnsigned(const std::vector<unsigned int>& digits);

        /*!
         * \brief Copy constructor.
         */
        BigUnsigned(const BigUnsigned& o);

        /*!
         * \brief Move constructor.
         */
        BigUnsigned(BigUnsigned&& o);

        /*!
         * \brief Copy assignment operator.
         */
        BigUnsigned& operator=(const BigUnsigned& o);

        /*!
         * \brief Addition operator.
         */
        BigUnsigned operator+(const BigUnsigned& o) const;

        /*!
         * \brief Compound multiplication assignment operator.
         */
        BigUnsigned& operator+=(const BigUnsigned& o);

        /*!
         * \brief Multiplication operator.
         */
        BigUnsigned operator*(unsigned int o) const;

        /*!
         * \brief Compound multiplication assignment operator.
         */
        BigUnsigned& operator*=(const BigUnsigned& o);

        /*!
         * \brief Comparison operator.
         */
        bool operator<(const BigUnsigned& o) const;

        /*!
         * \brief Get digits representing this number.
         *
         * \returns Digits making up arbitrary-precision unsigned int.
         */
        const std::vector<unsigned int>& get_digits() const;

        /*!
         * \brief Get the size (number of digits) of this number.
         *
         * \returns Size of this number.
         */
        unsigned int size() const;


      private:
        std::vector<unsigned int> digits_; //!< Digits representing this number

    };

    std::ostream& operator<<(std::ostream&, const BigUnsigned&);

  }
}

#endif // CANNON_MATH_BIGNUM_H
