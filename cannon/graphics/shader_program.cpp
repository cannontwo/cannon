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
    log_error("Shader object is", gl_shader_program_);
    throw std::runtime_error("Could not compile shader source.");
  }
}

void ShaderProgram::activate() {
  glUseProgram(gl_shader_program_);
}

void ShaderProgram::deactivate() {
  glUseProgram(0);
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
  deactivate();
}

void ShaderProgram::set_uniform(const std::string& name, float value, bool verbose) {
  activate();
  int location = get_uniform_loc_(name, verbose);
  glUniform1f(location, value);
  deactivate();
}

void ShaderProgram::set_uniform(const std::string& name, Vector4f value, bool verbose) {
  activate();
  int location = get_uniform_loc_(name, verbose);
  if (location != -1)
    glUniform4f(location, value[0], value[1], value[2], value[3]);
  deactivate();
}

void ShaderProgram::set_uniform(const std::string& name, Matrix4f value, bool verbose) {
  activate();
  int location = get_uniform_loc_(name, verbose);
  if (location != -1)
    glUniformMatrix4fv(location, 1, GL_FALSE, value.data());
  deactivate();
}

void ShaderProgram::reload() {
  glDeleteProgram(gl_shader_program_);
  init();
  attach_vertex_shader(v_src_, v_libs_);
  attach_fragment_shader(f_src_, f_libs_);
  link();
}

void ShaderProgram::write_imgui() {
  if (ImGui::TreeNode(name_.c_str())) {
    ImGui::InputText("Vertex Shader", &v_src_);
    for (unsigned int i = 0; i < v_libs_.size(); i++) {
      ImGui::PushID(i);
      std::string l = "Vertex Lib " + std::to_string(i);
      ImGui::InputText(l.c_str(), &v_libs_[i]);
      ImGui::PopID();
    }

    ImGui::InputText("Fragment Shader", &f_src_);
    for (unsigned int i = 0; i < f_libs_.size(); i++) {
      ImGui::PushID(v_libs_.size() + i);
      std::string l = "Fragment Lib " + std::to_string(i);
      ImGui::InputText(l.c_str(), &f_libs_[i]);
      ImGui::PopID();
    }

    if (ImGui::Button("Reload")) {
      reload();
    }
    ImGui::TreePop();
  }
}

void ShaderProgram::attach_vertex_shader(const std::string& v_src,
    const std::vector<std::string>& v_libs) {
  v_src_ = v_src;
  v_libs_ = v_libs;

  attach_shader(load_vertex_shader(v_src, v_libs_));
}

void ShaderProgram::attach_fragment_shader(const std::string& f_src,
    const std::vector<std::string>& f_libs) {
  f_src_ = f_src;
  f_libs_ = f_libs;

  attach_shader(load_fragment_shader(f_src, f_libs_));
}
