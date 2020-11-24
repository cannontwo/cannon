#include <thread>

#include <cannon/graphics/deferred_renderer.hpp>

using namespace cannon::graphics;

void test() {
  DeferredRenderer r;

  r.viewer.spawn_model("assets/test/test.obj");
  r.viewer.c.set_pos({5.0, 5.0, 10.0});
  r.viewer.c.set_direction({0.5, 0.75, 1.0});

  r.render_loop([](){});
}

int main() {
  std::thread render_thread(test);

  // There appears to be a driver bug that causes a segfault on thread destruction
  render_thread.join();
}
