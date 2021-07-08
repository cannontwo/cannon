list(APPEND MATH_SOURCES
  finite_diff_derivative.cpp
  barycentric_coords.cpp
  multivariate_normal.cpp
  func/rosenbrock.cpp
  random_double.cpp
  perlin.cpp
  primes.cpp
  bignum.cpp
  divisors.cpp
  to_decimal.cpp
  )

add_library(math OBJECT ${MATH_SOURCES})
