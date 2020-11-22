#include <cannon/graphics/fragment_shader.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

int main() {
  Window w;
  OpenGLState s;

  const char *str = BASIC_FRAGMENT_SHADER.c_str();

  FragmentShader f(&str);
  OpenGLState s1;
  assert(s == s1);
}
