list(APPEND CONTROL_SOURCES
  affine_controller.cpp
  )

add_library(control OBJECT ${CONTROL_SOURCES})
