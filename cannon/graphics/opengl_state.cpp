#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

bool cannon::graphics::operator==(const OpenGLState& lhs, const OpenGLState& rhs) {
  bool ret = true;

  ret = ret && (lhs.active_texture == rhs.active_texture);
  ret = ret && (lhs.program == rhs.program);
  ret = ret && (lhs.texture == rhs.texture);
  ret = ret && (lhs.cubemap_texture == rhs.cubemap_texture);
  ret = ret && (lhs.sampler == rhs.sampler);
  ret = ret && (lhs.array_buffer == rhs.array_buffer);
  ret = ret && (lhs.vertex_array_object == rhs.vertex_array_object);
  ret = ret && (lhs.element_buffer == rhs.element_buffer);
  ret = ret && (lhs.draw_buffer == rhs.draw_buffer);
  ret = ret && (lhs.read_buffer == rhs.read_buffer);

  ret = ret && (lhs.viewport[0] == rhs.viewport[0]);
  ret = ret && (lhs.viewport[1] == rhs.viewport[1]);
  ret = ret && (lhs.viewport[2] == rhs.viewport[2]);
  ret = ret && (lhs.viewport[3] == rhs.viewport[3]);

  ret = ret && (lhs.scissor_box[0] == rhs.scissor_box[0]);
  ret = ret && (lhs.scissor_box[1] == rhs.scissor_box[1]);
  ret = ret && (lhs.scissor_box[2] == rhs.scissor_box[2]);
  ret = ret && (lhs.scissor_box[3] == rhs.scissor_box[3]);

  ret = ret && (lhs.blend_src_rgb == rhs.blend_src_rgb);
  ret = ret && (lhs.blend_dst_rgb == rhs.blend_dst_rgb);
  ret = ret && (lhs.blend_src_alpha == rhs.blend_src_alpha);
  ret = ret && (lhs.blend_dst_alpha == rhs.blend_dst_alpha);
  ret = ret && (lhs.blend_equation_rgb == rhs.blend_equation_rgb);
  ret = ret && (lhs.blend_equation_alpha == rhs.blend_equation_alpha);

  ret = ret && (lhs.enable_blend == rhs.enable_blend);
  ret = ret && (lhs.enable_cull_face == rhs.enable_cull_face);
  ret = ret && (lhs.enable_depth_test == rhs.enable_depth_test);
  ret = ret && (lhs.enable_scissor_test == rhs.enable_scissor_test);
  ret = ret && (lhs.enable_primitive_restart == rhs.enable_primitive_restart);

  return ret;
}
