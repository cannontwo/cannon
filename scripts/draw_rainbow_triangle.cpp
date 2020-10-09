#include <glad/glad.h>

#include <cannon/graphics/window.hpp>
#include <cannon/graphics/vertex_color_buffer.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::graphics;
using namespace cannon::log;

int main() {
  Window w;
  
  MatrixXf vertices(3, 6);
              // positions        // colors
  vertices << -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
               0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
               0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f;

  auto vao = std::make_shared<VertexArrayObject>();
  VertexColorBuffer buf(vao);

  // Eigen stores matrices in column-major format, so we transpose because
  // OpenGL expects row-major
  buf.buffer(vertices); 
  buf.bind();

  buf.unbind();

  VertexShader v = load_vertex_shader("shaders/pass_color.vert");
  FragmentShader f = load_fragment_shader("shaders/pass_color.frag");

  ShaderProgram program;
  program.attach_shader(v);
  program.attach_shader(f);
  program.link();
  program.activate();

  buf.bind();
  w.render_loop([&] {
      buf.bind();
      program.activate();
      glDrawArrays(GL_TRIANGLES, 0, 3);
      });
}
