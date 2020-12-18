#ifdef CANNON_BUILD_GRAPHICS
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

int main() {
  Window w;
  w.display_text(25.0, 25.0, 1.0, "Perspective box", [](OverlayText& o){});
  w.enable_depth_test();

  MatrixXf vertices(8, 8);
               // positions           // colors          // texture coords 
  vertices <<  -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
               0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
               0.5f, 0.5f, -0.5f,     1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
               -0.5f, 0.5f, -0.5f,    1.0f, 1.0f, 1.0f,  0.0f, 1.0f,
               -0.5f, -0.5f, 0.5f,    1.0f, 1.0f, 1.0f,  0.0f, 0.0f,
               0.5f, -0.5f, 0.5f,     1.0f, 1.0f, 1.0f,  1.0f, 0.0f,
               0.5f, 0.5f, 0.5f,      1.0f, 1.0f, 1.0f,  1.0f, 1.0f,
               -0.5f, 0.5f, 0.5f,     1.0f, 1.0f, 1.0f,  0.0f, 1.0f;

  MatrixX3u indices(12, 3);
  indices << 0, 1, 2,
             2, 3, 0,
             4, 5, 6,
             6, 7, 4,
             7, 3, 0,
             0, 4, 7,
             6, 2, 1,
             1, 5, 6,
             0, 1, 5,
             5, 4, 0,
             3, 2, 6,
             6, 7, 3;


  auto vao = std::make_shared<VertexArrayObject>();
  VertexColorTexBuffer vbuf(vao);
  ElementBuffer ebuf(vao);

  vbuf.buffer(vertices); 
  vbuf.bind();

  ebuf.buffer(indices);
  ebuf.bind();

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

  // Set texture locations
  program.set_uniform("texture1", 0);
  program.set_uniform("texture2", 1);

  // Model matrix
  Affine3f model;
  model = AngleAxisf(to_radians(0.0f), Vector3f::UnitX());

  // View matrix
  Affine3f view;
  view = Translation3f(0.0f, 0.0f, -3.0f);

  //w.set_wireframe_mode();
  w.render_loop([&program, &w, &model, &view, &vbuf, &ebuf, &t0, &t1] {
    // Projection matrix
    Matrix4f projection = make_perspective_fov(to_radians(45.0f),
        (float)(w.width) / (float)(w.height), 0.1f, 100.0f);

    auto tmp_model = model * AngleAxisf((float)glfwGetTime() *
        to_radians(50.0f), Vector3f(0.5f, 1.0f, 0.0f));

    vbuf.bind();
    ebuf.bind();
    t0.bind();
    t1.bind();
    program.activate();
    program.set_uniform("model", tmp_model.matrix());
    program.set_uniform("view", view.matrix());
    program.set_uniform("projection", projection);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  });
  vbuf.unbind();
  ebuf.unbind();
}
#else
int main() {}
#endif
