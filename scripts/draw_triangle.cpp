#include <glad/glad.h>

#include <cannon/graphics/window.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::graphics;
using namespace cannon::log;

void render_func() {
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main() {
  init_glfw();

  Window w;
  
  MatrixX3f vertices(3, 3);
  vertices << -0.5f, -0.5f, 0.0f,
               0.5f, -0.5f, 0.0f,
               0.0f, 0.5f, 0.0f;

  VertexArrayObject vao;
  VertexBuffer buf(vao);

  // Eigen stores matrices in column-major format, so we transpose because
  // OpenGL expects row-major
  buf.buffer(vertices); 
  buf.bind();

  const char* v_src = BASIC_VERTEX_SHADER.c_str();
  const char* f_src = BASIC_FRAGMENT_SHADER.c_str();

  VertexShader v(&v_src);
  FragmentShader f(&f_src);

  ShaderProgram program;
  program.attach_shader(v);
  program.attach_shader(f);
  program.link();
  program.activate();
  program.set_uniform("uColor", Vector4f(1.0f, 0.0f, 0.0f, 1.0f));

  buf.bind();
  w.render_loop(render_func);
}
