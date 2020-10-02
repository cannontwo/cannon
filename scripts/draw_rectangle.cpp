#include <glad/glad.h>

#include <cannon/graphics/window.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/element_buffer.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::graphics;
using namespace cannon::log;

void render_func() {
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

int main() {
  init_glfw();

  auto w = create_window();

  MatrixX3f vertices(4, 3);
  vertices <<  0.5f, 0.5f, 0.0f, 
               0.5f, -0.5f, 0.0f,
              -0.5f, -0.5f, 0.0f,
              -0.5f, 0.5f, 0.0f;

  MatrixX3u indices(2, 3);
  indices << 0, 1, 3,
             1, 2, 3;

  VertexArrayObject vao;
  VertexBuffer vbuf(vao);
  ElementBuffer ebuf(vao);

  vbuf.buffer(vertices); 
  vbuf.bind();

  ebuf.buffer(indices);
  ebuf.bind();

  vao.bind();
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  const char* v_src = BASIC_VERTEX_SHADER.c_str();
  const char* f_src = BASIC_FRAGMENT_SHADER.c_str();

  VertexShader v(&v_src);
  FragmentShader f(&f_src);

  ShaderProgram program;
  program.attach_shader(v);
  program.attach_shader(f);
  program.link();
  program.activate();

  vbuf.bind();
  ebuf.bind();
  w.set_wireframe_mode();
  w.render_loop(render_func);
  vbuf.unbind();
  ebuf.unbind();
}
