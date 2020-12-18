#ifdef CANNON_BUILD_GRAPHICS

#include <thread>

#include <cannon/graphics/deferred_renderer.hpp>

using namespace cannon::graphics;

void test() {
  GLFWwindow *gl_window;
  {
  DeferredRenderer r;
  gl_window = r.viewer.w.get_gl_window();
  r.render_loop([](){});
  }
  terminate_opengl_context(gl_window);
}

int main() {
  std::thread render_thread(test);

  // There appears to be a driver bug that causes a segfault on thread destruction
  render_thread.join();
}
#else
int main() {}
#endif
