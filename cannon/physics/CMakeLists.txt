list(APPEND PHYSICS_SOURCES
  euler_integrator.cpp
  )

add_library(physics OBJECT ${PHYSICS_SOURCES})
