list(APPEND CONTROL_SOURCES
  affine_controller.cpp
  )

add_library(control ${CONTROL_SOURCES})
target_link_libraries(control log ml ${LIBRARIES})
