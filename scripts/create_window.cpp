#ifdef CANNON_BUILD_GRAPHICS

#include <cannon/graphics/window.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::graphics;
using namespace cannon::log;

void render_func() {
}

int main() {
  Window w;
  w.render_loop(render_func);
}
#else
int main() {}
#endif
