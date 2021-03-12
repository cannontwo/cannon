list(APPEND RAY_SOURCES
  write_ppm.cpp
  ray.cpp
  )

add_library(ray OBJECT ${RAY_SOURCES})
