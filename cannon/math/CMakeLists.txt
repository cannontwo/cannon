list(APPEND MATH_SOURCES
  finite_diff_derivative.cpp
  barycentric_coords.cpp
  multivariate_normal.cpp
  )

add_library(math ${MATH_SOURCES})
target_link_libraries(math)
