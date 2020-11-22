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

std::ostream& cannon::graphics::operator<<(std::ostream& os, const OpenGLState& state) {
  os << "Active Texture: " << state.active_texture << std::endl;
  os << "Program: " << state.program << std::endl;
  os << "Texture: " << state.texture << std::endl;
  os << "Cubemap Texture: " << state.cubemap_texture << std::endl;
  os << "Sampler: " << state.sampler << std::endl;
  os << "Array Buffer: " << state.array_buffer << std::endl;
  os << "Vertex Array Object: " << state.vertex_array_object << std::endl;
  os << "Element Buffer: " << state.element_buffer << std::endl;
  os << "Draw Buffer: " << state.draw_buffer << std::endl;
  os << "Read Buffer: " << state.read_buffer << std::endl;

  os << "Viewport[0]: " << state.viewport[0] << std::endl;
  os << "Viewport[1]: " << state.viewport[1] << std::endl;
  os << "Viewport[2]: " << state.viewport[2] << std::endl;
  os << "Viewport[3]: " << state.viewport[3] << std::endl;

  os << "Scissor Box[0]: " << state.scissor_box[0] << std::endl;
  os << "Scissor Box[1]: " << state.scissor_box[1] << std::endl;
  os << "Scissor Box[2]: " << state.scissor_box[2] << std::endl;
  os << "Scissor Box[3]: " << state.scissor_box[3] << std::endl;

  os << "Blend Src RGB: " << state.blend_src_rgb << std::endl;
  os << "Blend Dst RGB: " << state.blend_dst_rgb << std::endl;
  os << "Blend Src Alpha: " << state.blend_src_alpha << std::endl;
  os << "Blend Dst Alpha: " << state.blend_dst_alpha << std::endl;
  os << "Blend Equation RGB: " << state.blend_equation_rgb << std::endl;
  os << "Blend Equation Alpha: " << state.blend_equation_alpha << std::endl;

  os << "Enable Blend: " << state.enable_blend << std::endl;
  os << "Enable Cull Face: " << state.enable_cull_face << std::endl;
  os << "Enable Depth Test: " << state.enable_depth_test << std::endl;
  os << "Enable Scissor Test: " << state.enable_scissor_test << std::endl;
  os << "Enable Primitive Restart: " << state.enable_primitive_restart << std::endl;

  return os;
}
