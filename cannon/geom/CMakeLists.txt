list(APPEND GEOM_SOURCES
  kd_tree.cpp
  kd_tree_indexed.cpp
  )

add_library(geom ${GEOM_SOURCES})
target_link_libraries(geom log)
