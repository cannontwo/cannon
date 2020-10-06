#include <glad/glad.h>

#include <cannon/graphics/window.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/vertex_color_tex_buffer.hpp>
#include <cannon/graphics/element_buffer.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/texture.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::graphics;
using namespace cannon::log;

void render_func() {
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

int main() {
  init_glfw();

  Window w;

  MatrixXf vertices(4, 8);
               // positions      // colors         // texture coords 
  vertices <<  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
               0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
              -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
              -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f;

  MatrixX3u indices(2, 3);
  indices << 0, 1, 3,
             1, 2, 3;

  VertexArrayObject vao;
  VertexColorTexBuffer vbuf(vao);
  ElementBuffer ebuf(vao);

  vbuf.buffer(vertices); 
  vbuf.bind();

  ebuf.buffer(indices);
  ebuf.bind();

  vao.bind();

  Texture t("assets/container.jpg");  
  t.bind();

  VertexShader v = load_vertex_shader("shaders/pass_color_tex.vert");
  FragmentShader f = load_fragment_shader("shaders/pass_color_tex.frag");

  ShaderProgram program;
  program.attach_shader(v);
  program.attach_shader(f);
  program.link();
  program.activate();

  vbuf.bind();
  ebuf.bind();
  //w.set_wireframe_mode();
  w.render_loop([] {
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  });
  vbuf.unbind();
  ebuf.unbind();
}
