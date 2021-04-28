list(APPEND RAY_SOURCES
  write_ppm.cpp
  ray.cpp
  sphere.cpp
  hittable_list.cpp
  camera.cpp
  material.cpp
  )

add_library(ray OBJECT ${RAY_SOURCES})
