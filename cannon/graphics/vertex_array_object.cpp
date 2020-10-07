#include <cannon/graphics/vertex_array_object.hpp>

using namespace cannon::graphics;

void VertexArrayObject::bind() {
  glBindVertexArray(gl_vertex_array_object_);
}

void VertexArrayObject::unbind() {
  glBindVertexArray(0);
}

std::ostream& cannon::graphics::operator<<(std::ostream& os, const VertexArrayObject& vao) {
  os << "vao #" << vao.gl_vertex_array_object_;

  return os;
}
