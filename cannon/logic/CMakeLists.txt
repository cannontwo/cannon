list(APPEND LOGIC_SOURCES 
  form.cpp
  cnf.cpp
  read_dimacs_cnf.cpp
  )

add_library(logic ${LOGIC_SOURCES})
target_link_libraries(logic ${LIBRARIES})
