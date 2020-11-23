#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

int main() {
  Window w;
  OpenGLState s;
  VertexArrayObject vao;
  OpenGLState s1;
  assert(s == s1);

  vao.bind();
  OpenGLState s2;
  assert(s2.vertex_array_object != 0);
  s2.vertex_array_object = s1.vertex_array_object;
  assert(s2 == s1);

  vao.unbind();
  OpenGLState s3;
  assert(s3 == s1);

  vao.get_next_vertex_attribute_num();
  OpenGLState s4;
  assert(s4 == s1);

}
