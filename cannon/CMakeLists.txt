list(APPEND CANNON_SOURCES
  null.cpp
  )

add_subdirectory(math)
add_subdirectory(log)
add_subdirectory(logic)

add_library(cannon ${CANNON_SOURCES})
target_link_libraries(cannon 
  math 
  log 
  logic
  ${LIBRARIES})
