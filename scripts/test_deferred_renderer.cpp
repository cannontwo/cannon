#include <thread>

#include <cannon/graphics/deferred_renderer.hpp>

using namespace cannon::graphics;

void test() {
  DeferredRenderer r;
  r.render_loop([](){});
}

int main() {
  std::thread render_thread(test);

  // There appears to be a driver bug that causes a segfault on thread destruction
  render_thread.join();
}
