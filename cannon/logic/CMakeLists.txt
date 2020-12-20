list(APPEND LOGIC_SOURCES
  form.cpp
  cnf.cpp
  read_dimacs_cnf.cpp
  write_dimacs_cnf.cpp
  dpll.cpp
  circuit.cpp
  )

add_library(logic OBJECT ${LOGIC_SOURCES})
