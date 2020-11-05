#include <glad/glad.h>
#include <Eigen/Dense>
#include <stb_image/stb_image.h>
#include <memory>

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
#include <cannon/graphics/viewer_3d.hpp>
#include <cannon/log/registry.hpp>
#include <cannon/graphics/geometry/cube.hpp>
#include <cannon/graphics/geometry/textured_cube.hpp>

using namespace cannon::graphics;
using namespace cannon::log;

int main() {
  Viewer3D viewer;

  auto v = load_vertex_shader("shaders/mvp_normals.vert");
  auto f = load_fragment_shader("shaders/material_light.frag");
  auto program = std::make_shared<ShaderProgram>();
  program->attach_shader(v);
  program->attach_shader(f);
  program->link();

  auto v2 = load_vertex_shader("shaders/mvp_normals.vert");
  auto f2 = load_fragment_shader("shaders/material_light.frag");
  auto light_program = std::make_shared<ShaderProgram>();
  light_program->attach_shader(v2);
  light_program->attach_shader(f2);
  light_program->link();

  auto v3 = load_vertex_shader("shaders/mvp_normals_tex.vert");
  auto f3 = load_fragment_shader("shaders/material_light_tex.frag");
  auto textured_program = std::make_shared<ShaderProgram>();
  textured_program->attach_shader(v3);
  textured_program->attach_shader(f3);
  textured_program->link();

  auto c = std::make_shared<geometry::TexturedCube>(textured_program);
  auto c2 = std::make_shared<geometry::Cube>(program);
  auto light_cube = std::make_shared<geometry::Cube>(light_program);

  viewer.add_geom(c);
  viewer.add_geom(c2);
  viewer.add_geom(light_cube);

  Vector4f light_color;
  light_color << 1.0,
                 1.0,
                 1.0,
                 1.0;

  Vector4f light_pos;
  light_pos << 0.0,
               -1.0,
               2.0,
               1.0;

  geometry::Material light_material(light_color, {0.0, 0.0, 0.0, 1.0},
      {0.0, 0.0, 0.0, 1.0}, 32.0);
  light_cube->set_material(light_material);

  program->set_uniform("light.ambient", (Vector4f)(light_color * 0.2));
  program->set_uniform("light.diffuse", (Vector4f)(light_color * 0.5));
  program->set_uniform("light.specular", (Vector4f)(light_color * 1.0));
  program->set_uniform("light.position", light_pos);

  light_program->set_uniform("light.ambient", light_color);
  light_program->set_uniform("light.diffuse", light_color);
  light_program->set_uniform("light.specular", light_color);
  light_program->set_uniform("light.position", light_pos);

  textured_program->set_uniform("light.ambient", (Vector4f)(light_color * 0.2));
  textured_program->set_uniform("light.diffuse", (Vector4f)(light_color * 0.5));
  textured_program->set_uniform("light.specular", (Vector4f)(light_color * 1.0));
  textured_program->set_uniform("light.position", light_pos);

  Vector3f c_pos = viewer.c.get_pos();
  Vector4f tmp_pos;
  tmp_pos << c_pos[0],
             c_pos[1],
             c_pos[2],
             1.0;
  program->set_uniform("viewPos", tmp_pos);
  light_program->set_uniform("viewPos", tmp_pos);
  textured_program->set_uniform("viewPos", tmp_pos);

  c2->set_pos({2.0, 0.0, 0.0});
  light_cube->set_pos(light_pos.head(3));
  light_cube->set_scale(0.2);

  viewer.render_loop([&] {
    Vector4f light_color;
    light_color << std::sin(glfwGetTime() * 2.0),
                   std::sin(glfwGetTime() * 0.7),
                   std::sin(glfwGetTime() * 1.3),
                   1.0;

    program->set_uniform("light.ambient", (Vector4f)(light_color * 0.2));
    program->set_uniform("light.diffuse", (Vector4f)(light_color * 0.5));
    program->set_uniform("light.specular", (Vector4f)(light_color * 1.0));
    program->set_uniform("light.position", light_pos);

    light_program->set_uniform("light.ambient", light_color);
    light_program->set_uniform("light.diffuse", light_color);
    light_program->set_uniform("light.specular", light_color);
    light_program->set_uniform("light.position", light_pos);

    textured_program->set_uniform("light.ambient", (Vector4f)(light_color * 0.2));
    textured_program->set_uniform("light.diffuse", (Vector4f)(light_color * 0.5));
    textured_program->set_uniform("light.specular", (Vector4f)(light_color * 1.0));
    textured_program->set_uniform("light.position", light_pos);

    // Projection matrix
    c->set_rot(AngleAxisf((float)glfwGetTime() *
        to_radians(50.0f), Vector3f(0.5f, 1.0f, 0.0f).normalized()));
    c2->set_rot(AngleAxisf((float)glfwGetTime() *
        to_radians(50.0f), Vector3f(0.5f, 1.0f, 0.0f).normalized()));
  });
}
