list(APPEND GRAPHICS_SOURCES 
  window.cpp
  vertex_buffer.cpp
  vertex_color_buffer.cpp
  vertex_color_tex_buffer.cpp
  element_buffer.cpp
  vertex_shader.cpp
  fragment_shader.cpp
  shader_program.cpp
  vertex_array_object.cpp
  texture.cpp
  projection.cpp
  font.cpp
  character.cpp
  )

add_library(graphics ${GRAPHICS_SOURCES})
target_link_libraries(graphics log ${LIBRARIES})

# Copy shaders
ADD_CUSTOM_COMMAND(
  TARGET graphics POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/../../shaders
  COMMENT "Copying shaders" VERBATIM
  )

# Copy assets
ADD_CUSTOM_COMMAND(
  TARGET graphics POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/../../assets
  COMMENT "Copying assets" VERBATIM
  )

