#include <cannon/graphics/window.hpp>

using namespace cannon::graphics;

int main() {
  Window w;
  
  w.display_text(25.0, 25.0, 1.0, "Hello, world!");
  w.render_loop([] {});
}
