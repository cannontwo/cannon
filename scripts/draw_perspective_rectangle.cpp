#include <glad/glad.h>
#include <Eigen/Dense>
#include <stb_image/stb_image.h>

#include <cannon/graphics/window.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/vertex_color_tex_buffer.hpp>
#include <cannon/graphics/element_buffer.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/texture.hpp>
#include <cannon/graphics/projection.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::graphics;
using namespace cannon::log;

void render_func() {
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

float to_radians(float degrees) {
  return (degrees / 180.0f) * M_PI;
}

int main() {
  init_glfw();

  auto w = create_window();

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

  Texture t0("assets/container.jpg", false, GL_TEXTURE0);

  stbi_set_flip_vertically_on_load(true);
  Texture t1("assets/awesomeface.png", true, GL_TEXTURE1);  
  stbi_set_flip_vertically_on_load(false);

  VertexShader v = load_vertex_shader("shaders/mvp_color.vert");
  FragmentShader f = load_fragment_shader("shaders/mix_tex.frag");

  ShaderProgram program;
  program.attach_shader(v);
  program.attach_shader(f);
  program.link();
  program.activate();

  // Set texture locations
  program.set_uniform("texture1", 0);
  program.set_uniform("texture2", 1);

  // Model matrix
  Affine3f model;
  model = AngleAxisf(to_radians(-55.0f), Vector3f::UnitX());

  // View matrix
  Affine3f view;
  view = Translation3f(0.0f, 0.0f, -3.0f);

  vbuf.bind();
  ebuf.bind();
  //w.set_wireframe_mode();
  w.render_loop([&program, &w, &model, &view] {
    // Projection matrix
    Matrix4f projection = make_perspective_fov(to_radians(45.0f),
        (float)(w.width) / (float)(w.height), 0.1f, 100.0f);

    program.set_uniform("model", model.matrix());
    program.set_uniform("view", view.matrix());
    program.set_uniform("projection", projection);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  });
  //w.save_image("tmp.png");
  vbuf.unbind();
  ebuf.unbind();
}
