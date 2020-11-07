list(APPEND LOGIC_SOURCES 
  form.cpp
  cnf.cpp
  read_dimacs_cnf.cpp
  dpll.cpp
  )

add_library(logic ${LOGIC_SOURCES})
target_link_libraries(logic)

# Copy CNF formulas
ADD_CUSTOM_COMMAND(
  TARGET logic POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_SOURCE_DIR}/formulas ${CMAKE_CURRENT_BINARY_DIR}/../../formulas
  COMMENT "Copying formulas" VERBATIM
  )
