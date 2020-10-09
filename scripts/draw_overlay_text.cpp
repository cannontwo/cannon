#include <cannon/graphics/window.hpp>

using namespace cannon::graphics;

int main() {
  Window w;
  
  w.display_text(25.0, 25.0, 1.0, "Hello, world!", 
      [](OverlayText& o) {
        std::stringstream ss;
        ss << "Hello, world (" << glfwGetTime() << ")!";
        o.text.replace(o.text.begin(), o.text.end(), ss.str()); 
      });
  w.render_loop([] {});
}
