list(APPEND ML_SOURCES 
  rls.cpp
  piecewise_lstd.cpp
  adam.cpp
  )

add_library(ml OBJECT ${ML_SOURCES})
