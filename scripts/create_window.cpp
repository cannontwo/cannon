#include <cannon/graphics/window.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::graphics;
using namespace cannon::log;

void render_func() {
}

int main() {
  init_glfw();

  auto w = create_window();
  w.render_loop(render_func);
}
