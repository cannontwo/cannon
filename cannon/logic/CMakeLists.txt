list(APPEND LOGIC_SOURCES 
  form.cpp
  cnf.cpp
  read_dimacs_cnf.cpp
  dpll.cpp
  )

add_library(logic OBJECT ${LOGIC_SOURCES})

