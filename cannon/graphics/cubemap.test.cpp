#include <cannon/graphics/cubemap.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

int main() {
  Window w;

  OpenGLState s;

  Cubemap c;

  OpenGLState s1;
  assert(s == s1);

  c.bind(GL_TEXTURE0);
  OpenGLState s2;
  assert(s2.active_texture == GL_TEXTURE0);
  assert(s2.cubemap_texture != 0);

  c.unbind();
  OpenGLState s3;
  assert(s3 == s);
}
