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
  camera.cpp
  viewer_3d.cpp
  geometry/cube.cpp
  geometry/textured_cube.cpp
  geometry/mesh.cpp
  geometry/model.cpp
  geometry/plane.cpp
  framebuffer.cpp
  cubemap.cpp
  geometry/skybox.cpp
  geometry/screen_quad.cpp
  geometry/axes_hint.cpp
  geometry/sdf_volume.cpp
  deferred_renderer.cpp
  opengl_state.cpp
  )

add_library(graphics OBJECT ${GRAPHICS_SOURCES})
