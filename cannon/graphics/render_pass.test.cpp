#ifdef CANNON_BUILD_GRAPHICS
#include <catch2/catch.hpp>

#include <cannon/graphics/window.hpp>
#include <cannon/graphics/render_pass.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

TEST_CASE("RenderPass", "[graphics]") {
  Window w;
  auto f = std::make_shared<Framebuffer>();
  auto p = std::make_shared<ShaderProgram>();
  RenderPass rp("test", f, p, [](){});
  OpenGLState s;

  rp.run();
  OpenGLState s1;
  REQUIRE(s == s1);

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("Render Passes")) {
      rp.write_imgui();
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
  OpenGLState s2;
  REQUIRE(s2 == s1);

  rp.set_time_taken(0.01);
  OpenGLState s3;
  REQUIRE(s3 == s2);
}
#endif
