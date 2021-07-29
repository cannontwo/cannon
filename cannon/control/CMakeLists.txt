list(APPEND CONTROL_SOURCES
  affine_controller.cpp
  lqr.cpp
  in_port.cpp
  out_port.cpp
  blocks/math.cpp
  blocks/sources.cpp
  )

add_library(control OBJECT ${CONTROL_SOURCES})
