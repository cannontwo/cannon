#include <glad/glad.h>
#include <thread>

#include <imgui.h>
#include <imgui_stdlib.h>

#include <cannon/graphics/viewer_3d.hpp>
#include <cannon/graphics/framebuffer.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::graphics;
using namespace cannon::log;

void test() {
  GLFWwindow *gl_window;
  {
  std::vector<std::string> faces;
  faces.push_back("assets/skybox/right.jpg");
  faces.push_back("assets/skybox/left.jpg");
  faces.push_back("assets/skybox/top.jpg");
  faces.push_back("assets/skybox/bottom.jpg");
  faces.push_back("assets/skybox/front.jpg");
  faces.push_back("assets/skybox/back.jpg");

  Viewer3D viewer;
  gl_window = viewer.w.get_gl_window();
  viewer.set_skybox(faces);

  auto geom_program = std::make_shared<ShaderProgram>("geom_shader");
  geom_program->attach_vertex_shader("shaders/mvp_normals_tex.vert");
  geom_program->attach_fragment_shader("shaders/material_lights_tex.frag");
  geom_program->link();

  auto fb = std::make_shared<Framebuffer>(viewer.w.width, viewer.w.height, "geom framebuffer");
  auto rp = std::make_shared<RenderPass>("geom pass", fb, geom_program, [&](){
            Vector3f c_pos = viewer.c.get_pos();
            Vector4f tmp_pos;
            tmp_pos << c_pos[0],
                       c_pos[1],
                       c_pos[2],
                       1.0;
            geom_program->set_uniform("viewPos", tmp_pos);
            viewer.apply_light_collection(geom_program);
            viewer.draw_scene_geom(geom_program);
      });


  auto fb2 = std::make_shared<Framebuffer>(viewer.w.width, viewer.w.height, "screen framebuffer");
  auto rp2 = std::make_shared<RenderPass>("screen pass", fb2, fb2->quad->program, [&](){
      fb->bind_read();
      fb2->bind_draw();
      glBlitFramebuffer(0, 0, fb->width, fb->height, 0, 0, fb2->width,
          fb2->height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
      });

  viewer.add_render_pass(rp);
  viewer.add_render_pass(rp2);

  viewer.spawn_cube();
  viewer.render_loop_multipass([&] {});

  log_info("Render loop finished");
  }
  terminate_opengl_context(gl_window);
}

int main() {

  std::thread render_thread(test);

  // There appears to be a driver bug that causes a segfault on thread destruction
  render_thread.join();
}
