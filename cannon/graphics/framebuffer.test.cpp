#include <cannon/graphics/framebuffer.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

int main() {
  Window w;
  OpenGLState s;

  Framebuffer f; 
  OpenGLState s1;
  assert(s == s1);

  f.bind();
  OpenGLState s2;
  assert(s2.draw_buffer != GL_BACK);
  assert(s2.read_buffer != GL_BACK);
  s2.draw_buffer = s1.draw_buffer;
  s2.read_buffer = s1.read_buffer;
  assert(s1 == s2);

  f.unbind();
  OpenGLState s3; 
  assert(s == s3);

  f.bind_read();
  OpenGLState s4;
  assert(s4.read_buffer != GL_BACK);
  s4.read_buffer = s3.read_buffer;
  assert(s4 == s3);

  f.unbind_read();
  OpenGLState s5;
  assert(s == s5);

  f.bind_draw();
  OpenGLState s6;
  assert(s6.draw_buffer != GL_BACK);
  s6.draw_buffer = s5.draw_buffer;
  assert(s5 == s6);

  f.unbind_draw();
  OpenGLState s7;
  assert(s == s7);

  f.display();
  OpenGLState s8;
  assert(s8.draw_buffer != GL_BACK);
  assert(s8.read_buffer != GL_BACK);
  s8.draw_buffer = s7.draw_buffer;
  s8.read_buffer = s7.read_buffer;
  assert(s7 == s8);
  f.unbind();
  OpenGLState s9;

  f.draw_quad();
  OpenGLState s10;
  assert(s10 == s9);

  f.draw_quad(0);
  OpenGLState s11;
  assert(s11 == s10);

  f.attach_tex(std::make_shared<Texture>());
  OpenGLState s12;
  assert(s12 == s11);

  f.resize(100, 100);
  OpenGLState s13;
  assert(s13 == s12);
  
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("Framebuffer")) {
      f.write_imgui();
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }

  OpenGLState s14;
  assert(s14 == s13);
}
