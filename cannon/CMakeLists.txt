list(APPEND CANNON_SOURCES
  null.cpp
  )

add_subdirectory(math)
add_subdirectory(log)
add_subdirectory(logic)
add_subdirectory(graphics)
add_subdirectory(plot)
add_subdirectory(ml)
add_subdirectory(control)
add_subdirectory(physics)
add_subdirectory(geom)
add_subdirectory(research)

add_library(cannon ${CANNON_SOURCES})
target_link_libraries(cannon 
  math 
  log 
  logic
  graphics
  plot
  ml
  control
  physics
  geom
  research
  ${LIBRARIES})

