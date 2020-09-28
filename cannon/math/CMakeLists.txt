list(APPEND MATH_SOURCES
  finite_diff_derivative.cpp
  )

add_library(math ${MATH_SOURCES})
target_link_libraries(math ${LIBRARIES})
