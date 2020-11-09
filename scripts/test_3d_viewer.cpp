#include <glad/glad.h>
#include <Eigen/Dense>
#include <stb_image/stb_image.h>
#include <memory>
#include <random>

#include <imgui.h>
#include <imgui_stdlib.h>

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
#include <cannon/graphics/geometry/model.hpp>
#include <cannon/graphics/light.hpp>
#include <cannon/graphics/directional_light.hpp>
#include <cannon/graphics/point_light.hpp>
#include <cannon/graphics/light_collection.hpp>

using namespace cannon::graphics;
using namespace cannon::log;

int main() {
  Viewer3D viewer;

  auto textured_program = std::make_shared<ShaderProgram>("textured_cube_shader");
  textured_program->attach_vertex_shader("shaders/mvp_normals_tex.vert");
  textured_program->attach_fragment_shader("shaders/material_lights_tex.frag");
  textured_program->link();

  // Make random scene geometry
  for (int i = 0; i < 2; i++) {
    auto c = std::make_shared<geometry::TexturedCube>(textured_program);

    viewer.add_geom(c);

    Vector3f pos1 = Vector3f::Random() * 5.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-M_PI, M_PI);

    AngleAxisf rot1(dist(gen), Vector3f::Random().normalized());

    c->set_pos(pos1);
    c->set_rot(rot1);
  }

  viewer.render_loop([&] {
    Vector3f c_pos = viewer.c.get_pos();
    Vector4f tmp_pos;
    tmp_pos << c_pos[0],
               c_pos[1],
               c_pos[2],
               1.0;
    textured_program->set_uniform("viewPos", tmp_pos);
  });
}
