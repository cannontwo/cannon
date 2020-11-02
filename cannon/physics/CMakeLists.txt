list(APPEND PHYSICS_SOURCES
  euler_integrator.cpp
  )

add_library(physics ${PHYSICS_SOURCES})
target_link_libraries(physics log ${LIBRARIES})
