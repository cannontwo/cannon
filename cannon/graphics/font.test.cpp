#include <cannon/graphics/window.hpp>
#include <cannon/graphics/font.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

int main() {
  Window w;
  OpenGLState s;

  Font f;
  OpenGLState s1;
  assert(s == s1);
}
