list(APPEND GEOM_SOURCES
  kd_tree.cpp
  )

add_library(geom ${GEOM_SOURCES})
target_link_libraries(geom log ${LIBRARIES})
