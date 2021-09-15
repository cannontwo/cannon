list(APPEND ML_SOURCES 
  rls.cpp
  piecewise_lstd.cpp
  piecewise_recursive_lstd.cpp
  adam.cpp
  bfgs.cpp
  wolfe_conditions.cpp
  optimization_result.cpp
  pattern_search.cpp
  linear_programming.cpp
  lloyd.cpp
  )

add_library(ml OBJECT ${ML_SOURCES})
target_compile_options(ml PUBLIC -Wextra -Werror -Wno-error=deprecated-copy)
