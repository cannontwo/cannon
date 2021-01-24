list(APPEND PHYSICS_SOURCES
  euler_integrator.cpp
  rk4_integrator.cpp
  lagrangian_mechanics.cpp
  )

add_library(physics OBJECT ${PHYSICS_SOURCES})
