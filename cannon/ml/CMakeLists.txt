list(APPEND ML_SOURCES 
  rls.cpp
  piecewise_lstd.cpp
  adam.cpp
  bfgs.cpp
  wolfe_conditions.cpp
  optimization_result.cpp
  pattern_search.cpp
  )

add_library(ml OBJECT ${ML_SOURCES})
