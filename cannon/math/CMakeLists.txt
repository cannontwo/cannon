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
  digits.cpp
  palindromes.cpp
  polynomial.cpp
  fft.cpp
  interp.cpp
  rootfinding.cpp
  )

add_library(math OBJECT ${MATH_SOURCES})
target_compile_options(math PUBLIC -Wextra -Werror -Wno-error=deprecated-copy)
