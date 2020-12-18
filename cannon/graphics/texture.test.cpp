#ifdef CANNON_BUILD_GRAPHICS
#include <catch2/catch.hpp>

#include <cannon/graphics/texture.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

TEST_CASE("Texture", "[graphics]") {
  Window w;
  Texture t;
  OpenGLState s;

  t.bind();
  OpenGLState s1;
  REQUIRE(s1.texture != 0);
  REQUIRE(s1.active_texture == GL_TEXTURE0);
  s1.texture = s.texture;
  s1.active_texture = s.active_texture;
  REQUIRE(s == s1);

  t.bind(GL_TEXTURE1);
  OpenGLState s2;
  REQUIRE(s2.texture != 0);
  REQUIRE(s2.active_texture == GL_TEXTURE1);
  s2.texture = s.texture;
  s2.active_texture = s.active_texture;
  REQUIRE(s == s2);

  t.unbind();
  OpenGLState s3;
  REQUIRE(s3 == s);

  t.unbind(GL_TEXTURE1);
  OpenGLState s4;
  REQUIRE(s4.active_texture == GL_TEXTURE1);
  s4.active_texture = s.active_texture;
  REQUIRE(s == s4);

  t.unbind();

  t.framebuffer_bind();
  OpenGLState s5;
  REQUIRE(s5 == s);

  t.set_wrap_repeat();
  OpenGLState s6;
  REQUIRE(s6 == s);

  t.set_wrap_mirrored_repeat();
  OpenGLState s7;
  REQUIRE(s7 == s);

  t.set_wrap_clamp_edge();
  OpenGLState s8;
  REQUIRE(s8 == s);

  t.set_wrap_clamp_border();
  OpenGLState s9;
  REQUIRE(s9 == s);

  t.set_filter_linear();
  OpenGLState s10;
  REQUIRE(s10 == s);

  t.set_filter_nearest();
  OpenGLState s11;
  REQUIRE(s11 == s);

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
  REQUIRE(s == s12);

}
#endif
