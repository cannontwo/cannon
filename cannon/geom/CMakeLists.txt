list(APPEND GEOM_SOURCES
  kd_tree.cpp
  kd_tree_indexed.cpp
  )

add_library(geom OBJECT ${GEOM_SOURCES})
