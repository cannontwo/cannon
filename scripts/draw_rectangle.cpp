#ifdef CANNON_BUILD_GRAPHICS

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
  Window w;

  MatrixX3f vertices(4, 3);
  vertices <<  0.5f, 0.5f, 0.0f, 
               0.5f, -0.5f, 0.0f,
              -0.5f, -0.5f, 0.0f,
              -0.5f, 0.5f, 0.0f;

  MatrixX3u indices(2, 3);
  indices << 0, 1, 3,
             1, 2, 3;

  auto vao = std::make_shared<VertexArrayObject>();
  VertexBuffer vbuf(vao);
  ElementBuffer ebuf(vao);

  vbuf.buffer(vertices); 
  vbuf.bind();

  ebuf.buffer(indices);
  ebuf.bind();

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
  //w.set_wireframe_mode();
  w.render_loop([&] {
    program.activate();
    vbuf.bind();
    ebuf.bind();
    float t = glfwGetTime();
    float g = (sin(t) / 2.0f) + 0.5f;
    Vector4f v;
    v << 0.0f, g, 0.0f, 1.0f;
    program.set_uniform("uColor", v);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  });
  vbuf.unbind();
  ebuf.unbind();
}
#else
int main() {}
#endif
