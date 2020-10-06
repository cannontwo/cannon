#include <cannon/graphics/shader_program.hpp>

using namespace cannon::graphics;

void ShaderProgram::link() {
  glLinkProgram(gl_shader_program_);

  // Check for shader compilation error
  int success;
  char info[512];
  glGetProgramiv(gl_shader_program_, GL_LINK_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(gl_shader_program_, 512, NULL, info);
    log_error(info);
    throw std::runtime_error("Could not compile shader source.");
  }
}

void ShaderProgram::activate() {
  glUseProgram(gl_shader_program_);
}

int ShaderProgram::get_uniform_loc_(const std::string &name) {
  int location = glGetUniformLocation(gl_shader_program_, name.c_str());
  if (location == -1) 
    throw std::runtime_error("Could not locate uniform");

  return location;
}

void ShaderProgram::set_uniform(const std::string& name, int value) {
  int location = get_uniform_loc_(name);
  activate();
  glUniform1i(location, value);
}

void ShaderProgram::set_uniform(const std::string& name, Vector4f value) {
  int location = get_uniform_loc_(name);
  activate();
  glUniform4f(location, value[0], value[1], value[2], value[3]);
}

void ShaderProgram::set_uniform(const std::string& name, Matrix4f value) {
  int location = get_uniform_loc_(name);
  activate();
  glUniformMatrix4fv(location, 1, GL_FALSE, value.data());
}
