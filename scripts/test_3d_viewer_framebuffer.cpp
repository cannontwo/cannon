#include <glad/glad.h>
#include <thread>

#include <imgui.h>
#include <imgui_stdlib.h>

#include <cannon/graphics/viewer_3d.hpp>
#include <cannon/graphics/framebuffer.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::graphics;
using namespace cannon::log;

void test() {
  Viewer3D viewer;
  auto fb = std::make_shared<Framebuffer>(viewer.w.width, viewer.w.height);
  viewer.add_shader(fb->quad_program);
  viewer.w.render_to_framebuffer(fb);

  viewer.spawn_cube();

  viewer.render_loop([&] {});

  log_info("Render loop finished");
}

int main() {

  std::thread render_thread(test);

  // There appears to be a driver bug that causes a segfault on thread destruction
  render_thread.join();
}
