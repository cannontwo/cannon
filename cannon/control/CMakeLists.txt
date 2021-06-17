list(APPEND CONTROL_SOURCES
  affine_controller.cpp
  lqr.cpp
  )

add_library(control OBJECT ${CONTROL_SOURCES})
