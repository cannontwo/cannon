list(APPEND GEOM_SOURCES
  kd_tree.cpp
  kd_tree_indexed.cpp
  extract_triangles.cpp
  graph.cpp
  trajectory.cpp
  )

add_library(geom OBJECT ${GEOM_SOURCES})
target_compile_options(geom PUBLIC -Wextra -Werror -Wno-error=deprecated-copy)
