#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

int main() {
  Window w;
  OpenGLState s;

  ShaderProgram p;
  OpenGLState s1;
  assert(s == s1);

  p.attach_vertex_shader("shaders/mvp_normals_tex.vert");
  OpenGLState s2;
  assert(s2 == s1);

  p.attach_fragment_shader("shaders/deferred_lighting.frag");
  OpenGLState s3;
  assert(s3 == s2);

  p.link();
  OpenGLState s4;
  assert(s4 == s3);

  p.reload();
  OpenGLState s5;
  assert(s5 == s4);

  p.activate();
  OpenGLState s6;
  assert(s6.program != 0);
  s6.program = s5.program;
  assert(s6 == s5);

  p.deactivate();
  OpenGLState s7;
  assert(s7 == s5);

  p.set_uniform("tmp", 1);
  OpenGLState s8;
  assert(s8 == s7);

  p.set_uniform("tmp1", 1.0f);
  OpenGLState s9;
  assert(s9 == s8);

  Vector4f v;
  v << 0.0, 0.0, 0.0, 0.0;
  p.set_uniform("tmp2", v);
  OpenGLState s10;
  assert(s10 == s9);

  Matrix4f m;
  m << 0.0, 0.0, 0.0, 0.0,
       0.0, 0.0, 0.0, 0.0,
       0.0, 0.0, 0.0, 0.0,
       0.0, 0.0, 0.0, 0.0;
  p.set_uniform("tmp3", m);
  OpenGLState s11;
  assert(s11 == s10);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("Render Passes")) {
      p.write_imgui();
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
  OpenGLState s12;
  assert(s12 == s11);
}
