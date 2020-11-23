#include <cannon/graphics/geometry/textured_cube.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

int main() {
  Window w;
  OpenGLState s;

  auto textured_program = std::make_shared<ShaderProgram>("textured_cube_shader");
  textured_program->attach_vertex_shader("shaders/mvp_normals_tex.vert");
  textured_program->attach_fragment_shader("shaders/material_lights_tex.frag");
  textured_program->link();

  geometry::TexturedCube c(textured_program);
  OpenGLState s1;
  assert(s == s1);

  Matrix4f tmp = Matrix4f::Zero();
  c.draw(tmp, tmp);
  OpenGLState s2;
  assert(s == s2);

  auto p = std::make_shared<ShaderProgram>();
  p->attach_vertex_shader("shaders/mvp_uniform_color.vert");
  p->attach_fragment_shader("shaders/pass_color.frag");
  p->link();

  c.draw(p, tmp, tmp);
  OpenGLState s3;
  assert(s == s3);
}
