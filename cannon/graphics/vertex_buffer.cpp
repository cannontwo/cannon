#include <cannon/graphics/vertex_buffer.hpp>

using namespace cannon::graphics;

void VertexBuffer::init(std::shared_ptr<VertexArrayObject> vao) {
  assert(vao_ == nullptr);
  vao_ = vao;

  vao_->bind();
  glGenBuffers(1, &gl_vertex_buffer_object_);

  gl_vertex_attribute_num_ = vao_->get_next_vertex_attribute_num();
  vao_->unbind();
}

void VertexBuffer::bind() const {
  if (vao_ == nullptr)
    throw std::runtime_error("Buffer used without initialized vao.");

  vao_->bind();
  glBindBuffer(GL_ARRAY_BUFFER, gl_vertex_buffer_object_);
}

void VertexBuffer::unbind() const {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  if (vao_ != nullptr) 
    vao_->unbind();
}

void VertexBuffer::buffer(MatrixX3f vertices) {
  bind();

  // Eigen stores matrices in column-major format, so we transpose because
  // OpenGL expects row-major
  Matrix3Xf tmp_vertices;
  tmp_vertices.noalias() = vertices.transpose().eval();

  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tmp_vertices.size(), 
      tmp_vertices.data(), GL_DYNAMIC_DRAW);

  set_vertex_attribute_pointer(3);
  unbind();
}

void VertexBuffer::buffer(MatrixX2f vertices) {
  bind();

  // Eigen stores matrices in column-major format, so we transpose because
  // OpenGL expects row-major
  Matrix2Xf tmp_vertices;
  tmp_vertices.noalias() = vertices.transpose().eval();

  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tmp_vertices.size(), 
      tmp_vertices.data(), GL_DYNAMIC_DRAW);

  set_vertex_attribute_pointer(2);
  unbind();
}

void VertexBuffer::buffer(MatrixX4f vertices) {
  bind();

  // Eigen stores matrices in column-major format, so we transpose because
  // OpenGL expects row-major
  Matrix4Xf tmp_vertices;
  tmp_vertices.noalias() = vertices.transpose().eval();

  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * tmp_vertices.size(), 
      tmp_vertices.data(), GL_DYNAMIC_DRAW);

  set_vertex_attribute_pointer(4);
  unbind();
}

void VertexBuffer::replace(MatrixX2f vertices) {
  bind();

  int size;
  glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE,  &size);

  if (size == sizeof(float) * vertices.size()) {
    // Eigen stores matrices in column-major format, so we transpose because
    // OpenGL expects row-major
    Matrix2Xf tmp_vertices;
    tmp_vertices.noalias() = vertices.transpose().eval();
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * tmp_vertices.size(), tmp_vertices.data());
  } else {
    buffer(vertices);
  }
  unbind();
}

void VertexBuffer::replace(MatrixX3f vertices) {
  bind();

  int size;
  glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE,  &size);

  if (size == sizeof(float) * vertices.size()) {
    // Eigen stores matrices in column-major format, so we transpose because
    // OpenGL expects row-major
    Matrix3Xf tmp_vertices;
    tmp_vertices.noalias() = vertices.transpose().eval();
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * tmp_vertices.size(), tmp_vertices.data());
  } else {
    buffer(vertices);
  }
  unbind();
}

void VertexBuffer::replace(MatrixX4f vertices) {
  bind();

  int size;
  glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE,  &size);

  if (size == sizeof(float) * vertices.size()) {
    // Eigen stores matrices in column-major format, so we transpose because
    // OpenGL expects row-major
    Matrix4Xf tmp_vertices;
    tmp_vertices.noalias() = vertices.transpose().eval();
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * tmp_vertices.size(), tmp_vertices.data());
  } else {
    buffer(vertices);
  }
  unbind();
}

void VertexBuffer::set_vertex_attribute_pointer(int dim) {
  glVertexAttribPointer(gl_vertex_attribute_num_, dim, GL_FLOAT, GL_FALSE, dim * sizeof(float), (void *)0);
  glEnableVertexAttribArray(gl_vertex_attribute_num_);
}

// Free Functions
std::ostream& cannon::graphics::operator<<(std::ostream& os, const VertexBuffer& buf) {
  os << "buffer #" << buf.gl_vertex_buffer_object_ << ", attr=" << buf.gl_vertex_attribute_num_;

  return os;
}

