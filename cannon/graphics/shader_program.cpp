#include <cannon/graphics/shader_program.hpp>

using namespace cannon::graphics;

void ShaderProgram::link() {
  glLinkProgram(gl_shader_program_);

  // Check for shader compilation error
  int success;
  char info[512];
  glGetShaderiv(gl_shader_program_, GL_LINK_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(gl_shader_program_, 512, NULL, info);
    log_error(info);
    throw std::runtime_error("Could not compile shader source.");
  }
}

void ShaderProgram::activate() {
  glUseProgram(gl_shader_program_);
}
