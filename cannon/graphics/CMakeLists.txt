list(APPEND GRAPHICS_SOURCES 
  window.cpp
  )

add_library(graphics ${GRAPHICS_SOURCES})
target_link_libraries(graphics ${LIBRARIES})
