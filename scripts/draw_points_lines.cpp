#include <glad/glad.h>
#include <Eigen/Dense>

#include <cannon/graphics/window.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>

using namespace Eigen;
using namespace cannon::graphics;

int main() {
  Window w;

  glPointSize(15.0);
  glLineWidth(5.0);

  MatrixX2f points(10, 2);
  points = MatrixX2f::Random(10, 2);

  VertexArrayObject vao1;
  VertexBuffer buf1(vao1);
  buf1.buffer(points);

  MatrixX2f lines(4, 2);
  lines << -1.0, 0.0,
           1.0, 0.0,
           0.0, -1.0,
           0.0, 1.0;

  VertexArrayObject vao2;
  VertexBuffer buf2(vao2);
  buf2.buffer(lines);

  const char* v_src = BASIC_VERTEX_SHADER_2D.c_str();
  const char* f_src = BASIC_FRAGMENT_SHADER.c_str();

  VertexShader v(&v_src);
  FragmentShader f(&f_src);

  ShaderProgram program;
  program.attach_shader(v);
  program.attach_shader(f);
  program.link();
  program.activate();
  program.set_uniform("uColor", Vector4f(0.0f, 0.0f, 0.0f, 1.0f));

  w.set_clear_color(Vector4f(1.0, 1.0, 1.0, 1.0));

  w.render_loop([&buf1, &buf2] {

      buf1.bind();
      glDrawArrays(GL_POINTS, 0, 10);

      buf2.bind();
      glDrawArrays(GL_LINES, 0, 4);
      });
}
