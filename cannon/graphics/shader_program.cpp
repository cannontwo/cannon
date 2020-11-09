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

int ShaderProgram::get_uniform_loc_(const std::string &name, bool verbose) {
  int location = glGetUniformLocation(gl_shader_program_, name.c_str());
  if (location == -1 && verbose) {
    log_error("Couldn't find uniform", name, "in shader", gl_shader_program_);
  }

  return location;
}

void ShaderProgram::set_uniform(const std::string& name, int value, bool verbose) {
  activate();
  int location = get_uniform_loc_(name, verbose);
  if (location != -1)
    glUniform1i(location, value);
}

void ShaderProgram::set_uniform(const std::string& name, float value, bool verbose) {
  activate();
  int location = get_uniform_loc_(name, verbose);
  glUniform1f(location, value);
}

void ShaderProgram::set_uniform(const std::string& name, Vector4f value, bool verbose) {
  activate();
  int location = get_uniform_loc_(name, verbose);
  if (location != -1)
    glUniform4f(location, value[0], value[1], value[2], value[3]);
}

void ShaderProgram::set_uniform(const std::string& name, Matrix4f value, bool verbose) {
  activate();
  int location = get_uniform_loc_(name, verbose);
  if (location != -1)
    glUniformMatrix4fv(location, 1, GL_FALSE, value.data());
}

void ShaderProgram::reload() {
  glDeleteProgram(gl_shader_program_);
  init();
  attach_vertex_shader(v_src_);
  attach_fragment_shader(f_src_);
  link();
}

void ShaderProgram::write_imgui() {
  if (ImGui::BeginMenu(name_.c_str())) {
    ImGui::InputText("Vertex Shader", &v_src_);
    ImGui::InputText("Fragment Shader", &f_src_);
    if (ImGui::Button("Reload")) {
      reload();
    }
    ImGui::EndMenu();
  }
}

void ShaderProgram::attach_vertex_shader(const std::string& v_src) {
  v_src_ = v_src;

  attach_shader(load_vertex_shader(v_src));
}

void ShaderProgram::attach_fragment_shader(const std::string& f_src) {
  f_src_ = f_src;

  attach_shader(load_fragment_shader(f_src));
}
