#include <catch2/catch.hpp>

#include <cannon/graphics/framebuffer.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

TEST_CASE("Framebuffer", "[graphics]") {
  Window w;
  OpenGLState s;

  Framebuffer f; 
  OpenGLState s1;
  REQUIRE(s == s1);

  f.bind();
  OpenGLState s2;
  REQUIRE(s2.draw_buffer != GL_BACK);
  REQUIRE(s2.read_buffer != GL_BACK);
  s2.draw_buffer = s1.draw_buffer;
  s2.read_buffer = s1.read_buffer;
  REQUIRE(s1 == s2);

  f.unbind();
  OpenGLState s3; 
  REQUIRE(s == s3);

  f.bind_read();
  OpenGLState s4;
  REQUIRE(s4.read_buffer != GL_BACK);
  s4.read_buffer = s3.read_buffer;
  REQUIRE(s4 == s3);

  f.unbind_read();
  OpenGLState s5;
  REQUIRE(s == s5);

  f.bind_draw();
  OpenGLState s6;
  REQUIRE(s6.draw_buffer != GL_BACK);
  s6.draw_buffer = s5.draw_buffer;
  REQUIRE(s5 == s6);

  f.unbind_draw();
  OpenGLState s7;
  REQUIRE(s == s7);

  f.display();
  OpenGLState s8;
  REQUIRE(s8.draw_buffer != GL_BACK);
  REQUIRE(s8.read_buffer != GL_BACK);
  s8.draw_buffer = s7.draw_buffer;
  s8.read_buffer = s7.read_buffer;
  REQUIRE(s7 == s8);
  f.unbind();
  OpenGLState s9;

  f.draw_quad();
  OpenGLState s10;
  REQUIRE(s10 == s9);

  f.draw_quad(0);
  OpenGLState s11;
  REQUIRE(s11 == s10);

  f.attach_tex(std::make_shared<Texture>());
  OpenGLState s12;
  REQUIRE(s12 == s11);

  f.resize(100, 100);
  OpenGLState s13;
  REQUIRE(s13 == s12);
  
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
  REQUIRE(s14 == s13);
}
