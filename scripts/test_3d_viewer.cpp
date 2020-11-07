#include <glad/glad.h>
#include <Eigen/Dense>
#include <stb_image/stb_image.h>
#include <memory>
#include <random>

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
#include <cannon/graphics/light.hpp>
#include <cannon/graphics/directional_light.hpp>
#include <cannon/graphics/point_light.hpp>
#include <cannon/graphics/light_collection.hpp>

using namespace cannon::graphics;
using namespace cannon::log;

int main() {
  Viewer3D viewer;

  auto v = load_vertex_shader("shaders/mvp_normals.vert");
  auto f = load_fragment_shader("shaders/material_lights.frag");
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
  auto f3 = load_fragment_shader("shaders/material_lights_tex.frag");
  auto textured_program = std::make_shared<ShaderProgram>();
  textured_program->attach_shader(v3);
  textured_program->attach_shader(f3);
  textured_program->link();

  // Make random scene geometry
  for (int i = 0; i < 5; i++) {
    auto c = std::make_shared<geometry::TexturedCube>(textured_program);
    auto c2 = std::make_shared<geometry::Cube>(program);

    viewer.add_geom(c);
    viewer.add_geom(c2);

    Vector3f pos1 = Vector3f::Random() * 5.0;
    Vector3f pos2 = Vector3f::Random() * 5.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-M_PI, M_PI);

    AngleAxisf rot1(dist(gen), Vector3f::Random().normalized());
    AngleAxisf rot2(dist(gen), Vector3f::Random().normalized());

    c->set_pos(pos1);
    c2->set_pos(pos2);

    c->set_rot(rot1);
    c->set_rot(rot2);
  }

  // Light setup


  //auto light = std::make_shared<Light>(light_color*0.2, light_color*0.5, light_color);
  
  // Make random lights
  LightCollection lc;
  Vector4f light_color;
  light_color << 0.5,
                 0.5,
                 0.5,
                 0.5;
  auto dl = std::make_shared<DirectionalLight>(light_color*0.2, light_color*0.5, light_color);
  dl->set_direction({-0.2, -1.0, -0.3, -1.0});
  lc.add_light(dl);

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dist(0.0, 1.0);
  for (int i = 0; i < 2; i++) {
    Vector4f light_color;
    Vector3f random_color;
    random_color[0] = dist(gen);
    random_color[1] = dist(gen);
    random_color[2] = dist(gen);
    random_color.normalize();
    random_color *= std::sqrt(3);

    light_color.head(3) = random_color;
    light_color[3] = 1.0;

    geometry::Material light_material(light_color, {0.0, 0.0, 0.0, 1.0},
        {0.0, 0.0, 0.0, 1.0}, 32.0);

    auto light = std::make_shared<PointLight>(light_color*0.2, light_color*0.5, light_color);
    auto light_cube = std::make_shared<geometry::Cube>(light_program);
    viewer.add_geom(light_cube);

    Vector4f light_pos;
    light_pos.head(3) = 5.0 * Vector3f::Random();
    light_pos[3] = 1.0;
    light->set_pos(light_pos);

    light_cube->set_material(light_material);
    light_cube->set_pos(light_pos.head(3));
    light_cube->set_scale(0.2);

    // Lights rendered saturated
    light->set_ambient(light_color);
    light->apply(light_cube);
    light->set_ambient(light_color * 0.2);

    lc.add_light(light);
  }

  viewer.apply_light_collection(lc);

  viewer.render_loop([&] {
    //Vector4f light_color;
    //light_color << std::sin(glfwGetTime() * 2.0),
    //               std::sin(glfwGetTime() * 0.7),
    //               std::sin(glfwGetTime() * 1.3),
    //               1.0;

    //light->set_ambient(light_color * 0.2);
    //light->set_diffuse(light_color * 0.5);
    //light->set_specular(light_color * 1.0);
    //
    //viewer.apply_light_collection(lc);
    //light->apply(light_cube);
    
    Vector3f c_pos = viewer.c.get_pos();
    Vector4f tmp_pos;
    tmp_pos << c_pos[0],
               c_pos[1],
               c_pos[2],
               1.0;
    program->set_uniform("viewPos", tmp_pos);
    light_program->set_uniform("viewPos", tmp_pos);
    textured_program->set_uniform("viewPos", tmp_pos);

    // Projection matrix
    //c->set_rot(AngleAxisf((float)glfwGetTime() *
    //    to_radians(50.0f), Vector3f(0.5f, 1.0f, 0.0f).normalized()));
    //c2->set_rot(AngleAxisf((float)glfwGetTime() *
    //    to_radians(50.0f), Vector3f(0.5f, 1.0f, 0.0f).normalized()));
    //c3->set_rot(AngleAxisf((float)glfwGetTime() *
    //    to_radians(50.0f), Vector3f(0.5f, 1.0f, 0.0f).normalized()));
  });
}
