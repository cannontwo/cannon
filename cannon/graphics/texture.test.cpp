#include <cannon/graphics/texture.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

int main() {
  Window w;
  Texture t;
  OpenGLState s;

  t.bind();
  OpenGLState s1;
  assert(s1.texture != 0);
  assert(s1.active_texture == GL_TEXTURE0);
  s1.texture = s.texture;
  s1.active_texture = s.active_texture;
  assert(s == s1);

  t.bind(GL_TEXTURE1);
  OpenGLState s2;
  assert(s2.texture != 0);
  assert(s2.active_texture == GL_TEXTURE1);
  s2.texture = s.texture;
  s2.active_texture = s.active_texture;
  assert(s == s2);

  t.unbind();
  OpenGLState s3;
  assert(s3 == s);

  t.unbind(GL_TEXTURE1);
  OpenGLState s4;
  assert(s4.active_texture == GL_TEXTURE1);
  s4.active_texture = s.active_texture;
  assert(s == s4);

  t.unbind();

  t.framebuffer_bind();
  OpenGLState s5;
  assert(s5 == s);

  t.set_wrap_repeat();
  OpenGLState s6;
  assert(s6 == s);

  t.set_wrap_mirrored_repeat();
  OpenGLState s7;
  assert(s7 == s);

  t.set_wrap_clamp_edge();
  OpenGLState s8;
  assert(s8 == s);

  t.set_wrap_clamp_border();
  OpenGLState s9;
  assert(s9 == s);

  t.set_filter_linear();
  OpenGLState s10;
  assert(s10 == s);

  t.set_filter_nearest();
  OpenGLState s11;
  assert(s11 == s);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("Textures")) {
      t.write_imgui();
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
  OpenGLState s12;
  assert(s == s12);

}
