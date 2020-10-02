list(APPEND GRAPHICS_SOURCES 
  window.cpp
  vertex_buffer.cpp
  element_buffer.cpp
  vertex_shader.cpp
  fragment_shader.cpp
  shader_program.cpp
  vertex_array_object.cpp
  )

add_library(graphics ${GRAPHICS_SOURCES})
target_link_libraries(graphics ${LIBRARIES})
