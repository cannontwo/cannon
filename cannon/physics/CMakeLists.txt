list(APPEND PHYSICS_SOURCES
  euler_integration.cpp
  )

add_library(physics ${CONTROL_SOURCES})
target_link_libraries(physics log ${LIBRARIES})
