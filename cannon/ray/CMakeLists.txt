list(APPEND RAY_SOURCES
  write_ppm.cpp
  ray.cpp
  sphere.cpp
  moving_sphere.cpp
  hittable_list.cpp
  camera.cpp
  material.cpp
  raytracer.cpp
  aabb.cpp
  bvh.cpp
  texture.cpp
  aa_rect.cpp
  hittable.cpp
  )

add_library(ray OBJECT ${RAY_SOURCES})
