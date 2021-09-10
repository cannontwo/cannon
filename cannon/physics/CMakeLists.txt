list(APPEND PHYSICS_SOURCES
  euler_integrator.cpp
  rk4_integrator.cpp
  lagrangian_mechanics.cpp
  )

add_library(physics OBJECT ${PHYSICS_SOURCES})
target_compile_options(physics PUBLIC -Wextra -Werror -Wno-error=deprecated-copy)
