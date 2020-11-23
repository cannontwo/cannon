#include <catch2/catch.hpp>

#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

TEST_CASE("ShaderProgram", "[graphics]") {
  Window w;
  OpenGLState s;

  ShaderProgram p;
  OpenGLState s1;
  REQUIRE(s == s1);

  p.attach_vertex_shader("shaders/mvp_normals_tex.vert");
  OpenGLState s2;
  REQUIRE(s2 == s1);

  p.attach_fragment_shader("shaders/deferred_lighting.frag");
  OpenGLState s3;
  REQUIRE(s3 == s2);

  p.link();
  OpenGLState s4;
  REQUIRE(s4 == s3);

  p.reload();
  OpenGLState s5;
  REQUIRE(s5 == s4);

  p.activate();
  OpenGLState s6;
  REQUIRE(s6.program != 0);
  s6.program = s5.program;
  REQUIRE(s6 == s5);

  p.deactivate();
  OpenGLState s7;
  REQUIRE(s7 == s5);

  p.set_uniform("tmp", 1);
  OpenGLState s8;
  REQUIRE(s8 == s7);

  p.set_uniform("tmp1", 1.0f);
  OpenGLState s9;
  REQUIRE(s9 == s8);

  Vector4f v;
  v << 0.0, 0.0, 0.0, 0.0;
  p.set_uniform("tmp2", v);
  OpenGLState s10;
  REQUIRE(s10 == s9);

  Matrix4f m;
  m << 0.0, 0.0, 0.0, 0.0,
       0.0, 0.0, 0.0, 0.0,
       0.0, 0.0, 0.0, 0.0,
       0.0, 0.0, 0.0, 0.0;
  p.set_uniform("tmp3", m);
  OpenGLState s11;
  REQUIRE(s11 == s10);

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
  REQUIRE(s12 == s11);
}
