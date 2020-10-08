#include <cannon/graphics/vertex_array_object.hpp>

using namespace cannon::graphics;

void VertexArrayObject::bind() {
  glBindVertexArray(gl_vertex_array_object_);
}

void VertexArrayObject::unbind() {
  glBindVertexArray(0);
}

int VertexArrayObject::get_next_vertex_attribute_num() {
  int max;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max);

  if (declared_vertex_attribs_ >= max)
    throw std::runtime_error("Too many vertex attributes declared");

  declared_vertex_attribs_ += 1;
  return declared_vertex_attribs_ - 1;
}

// Operators
std::ostream& cannon::graphics::operator<<(std::ostream& os, const VertexArrayObject& vao) {
  os << "vao #" << vao.gl_vertex_array_object_;

  return os;
}

