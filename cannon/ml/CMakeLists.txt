list(APPEND ML_SOURCES 
  rls.cpp
  piecewise_lstd.cpp
  )

add_library(ml ${ML_SOURCES})
target_link_libraries(ml log ${LIBRARIES})
