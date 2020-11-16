#include <glad/glad.h>
#include <thread>

#include <imgui.h>
#include <imgui_stdlib.h>

#include <cannon/graphics/viewer_3d.hpp>
#include <cannon/graphics/framebuffer.hpp>
#include <cannon/log/registry.hpp>
#include <cannon/math/multivariate_normal.hpp>

using namespace cannon::graphics;
using namespace cannon::log;
using namespace cannon::math;

float lerp(float a, float b, float f) {
  return a + f * (b - a);
}

void test() {
  std::vector<std::string> faces;
  faces.push_back("assets/skybox/right.jpg");
  faces.push_back("assets/skybox/left.jpg");
  faces.push_back("assets/skybox/top.jpg");
  faces.push_back("assets/skybox/bottom.jpg");
  faces.push_back("assets/skybox/front.jpg");
  faces.push_back("assets/skybox/back.jpg");

  Viewer3D viewer;
  viewer.set_skybox(faces);

  auto gbuf_program = std::make_shared<ShaderProgram>("gbuffer_shader");
  gbuf_program->attach_vertex_shader("shaders/mvp_normals_gbuffer.vert");
  gbuf_program->attach_fragment_shader("shaders/gbuffer.frag");
  gbuf_program->link();

  std::vector<std::shared_ptr<Texture>> attachments;
  attachments.push_back(std::make_shared<Texture>(viewer.w.width,
        viewer.w.height, GL_RGBA32F, GL_FLOAT));
  attachments.push_back(std::make_shared<Texture>(viewer.w.width,
        viewer.w.height, GL_RGBA32F, GL_FLOAT));
  attachments.push_back(std::make_shared<Texture>(viewer.w.width,
        viewer.w.height, GL_RGBA32F, GL_FLOAT));

  viewer.w.set_clear_color({0.0, 0.0, 0.0, 1.0});

  auto fb = std::make_shared<Framebuffer>(attachments, viewer.w.width,
      viewer.w.height, "gbuffer framebuffer");
  auto rp = std::make_shared<RenderPass>("gbuffer pass", fb, gbuf_program, [&](){
            glDisable(GL_FRAMEBUFFER_SRGB); 
            viewer.disable_skybox();
            Vector3f c_pos = viewer.c.get_pos();
            Vector4f tmp_pos;
            tmp_pos << c_pos[0],
                       c_pos[1],
                       c_pos[2],
                       1.0;
            gbuf_program->set_uniform("viewPos", tmp_pos);
            viewer.apply_light_collection(gbuf_program);
            
            fb->color_attachments[0]->unbind();

            viewer.draw_scene_geom(gbuf_program, false);
      });

  std::vector<Vector4f> ssao_kernel;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> d(0.0, 1.0);
  for (int i = 0; i < 64; ++i) {
    float t1 = d(gen) * 2.0 - 1.0;
    float t2 = d(gen) * 2.0 - 1.0;
    float t3 = d(gen);

    Vector4f tmp;
    tmp[0] = t1;
    tmp[1] = t2;
    tmp[2] = t3;
    tmp[3] = 1.0;

    tmp.head(3).normalize();
    tmp.head(3) *= d(gen);

    float scale = float(i) / 64.0;
    scale = lerp(0.1f, 1.0f, scale * scale);
    tmp.head(3) *= scale;

    ssao_kernel.push_back(tmp);
  }

  std::vector<float> ssao_noise;
  for (int i = 0; i < 16; i++) {
    float t1 = d(gen) * 2.0 - 1.0;
    float t2 = d(gen) * 2.0 - 1.0;

    ssao_noise.push_back(t1);
    ssao_noise.push_back(t2);
    ssao_noise.push_back(0.0);
    ssao_noise.push_back(1.0);
  }


  auto noise_tex = std::make_shared<Texture>(4, 4, GL_RGBA32F, GL_FLOAT, ssao_noise.data());
  noise_tex->set_filter_nearest();
  noise_tex->set_wrap_repeat();

  auto ssao_program = std::make_shared<ShaderProgram>("ssao_shader");
  ssao_program->attach_vertex_shader("shaders/pass_pos_tex.vert");
  ssao_program->attach_fragment_shader("shaders/ssao.frag");
  ssao_program->link();

  std::vector<std::shared_ptr<Texture>> ssao_attachments;
  ssao_attachments.push_back(attachments[0]);
  ssao_attachments.push_back(attachments[1]);
  ssao_attachments.push_back(noise_tex);

  auto ssao_fb = std::make_shared<Framebuffer>(viewer.w.width, viewer.w.height, "ssao framebuffer");
  auto ssao_rp = std::make_shared<RenderPass>("ssao pass", ssao_fb, ssao_program, [&](){
        Vector3f c_pos = viewer.c.get_pos();
        Vector4f tmp_pos;
        tmp_pos << c_pos[0],
                   c_pos[1],
                   c_pos[2],
                   1.0;
        ssao_program->set_uniform("viewPos", tmp_pos);
      
        ssao_program->set_uniform("gPosition", 0);
        ssao_program->set_uniform("gNormal", 1);
        ssao_program->set_uniform("texNoise", 2);

        ssao_program->set_uniform("screen_width", (float)viewer.w.width);
        ssao_program->set_uniform("screen_height", (float)viewer.w.height);

        static float radius = 0.5;
        static float bias = 0.025;
        if (ImGui::BeginMainMenuBar()) {
          if (ImGui::BeginMenu("SSAO")) {
            ImGui::SliderFloat("radius", &radius, 0.0, 10.0);
            ImGui::SliderFloat("bias", &bias, 0.0, 1.0);
            ImGui::EndMenu();
          }
          ImGui::EndMainMenuBar();
        }
        ssao_program->set_uniform("radius", radius);
        ssao_program->set_uniform("bias", bias);

        // Set samples uniform
        for (int i = 0; i < 64; i++)
          ssao_program->set_uniform("samples[" + std::to_string(i) + "]", (Vector4f)ssao_kernel[i]);

        Matrix4f perspective = make_perspective_fov(to_radians(45.0f),
            (float)(viewer.w.width) / (float)(viewer.w.height), 0.1f, 50.0f);

        ssao_program->set_uniform("projection", perspective);
        ssao_program->set_uniform("view", viewer.c.get_view_mat());

        log_info("View dir is", viewer.c.get_direction());

        ssao_fb->quad->draw(ssao_program, ssao_attachments);
      });
  

  auto lighting_program = std::make_shared<ShaderProgram>("lighting_shader");
  lighting_program->attach_vertex_shader("shaders/pass_pos_tex.vert");
  lighting_program->attach_fragment_shader("shaders/deferred_lighting.frag");
  lighting_program->link();

  auto fb2 = std::make_shared<Framebuffer>(viewer.w.width, viewer.w.height, "lighting framebuffer");
  auto rp2 = std::make_shared<RenderPass>("lighting pass", fb2, lighting_program, [&](){
        Vector3f c_pos = viewer.c.get_pos();
        Vector4f tmp_pos;
        tmp_pos << c_pos[0],
                   c_pos[1],
                   c_pos[2],
                   1.0;
        lighting_program->set_uniform("viewPos", tmp_pos);
        lighting_program->set_uniform("shininess", (float)32.0);
        viewer.apply_light_collection(lighting_program);

        lighting_program->set_uniform("gPosition", 0);
        lighting_program->set_uniform("gNormal", 1);
        lighting_program->set_uniform("gAlbedoSpec", 2);

        fb2->quad->draw(lighting_program, attachments);
      });

  auto light_geom_program = std::make_shared<ShaderProgram>("light_geom_shader");
  light_geom_program->attach_vertex_shader("shaders/mvp_uniform_color.vert");
  light_geom_program->attach_fragment_shader("shaders/pass_color.frag");
  light_geom_program->link();

  auto fb3 = std::make_shared<Framebuffer>(viewer.w.width, viewer.w.height, "light geom framebuffer");
  auto rp3 = std::make_shared<RenderPass>("light geom pass", fb3, light_geom_program, [&]() {
        Vector3f c_pos = viewer.c.get_pos();
        Vector4f tmp_pos;
        tmp_pos << c_pos[0],
                   c_pos[1],
                   c_pos[2],
                   1.0;
        light_geom_program->set_uniform("uColor", Vector4f{1.0, 1.0, 1.0, 1.0});
        viewer.apply_light_collection(light_geom_program);

        fb2->bind_read();
        fb3->bind_draw();
        glBlitFramebuffer(0, 0, fb2->width, fb2->height, 0, 0, fb3->width,
            fb3->height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        fb3->bind();

        fb->bind_read();
        fb3->bind_draw();
        glBlitFramebuffer(0, 0, fb->width, fb->height, 0, 0, fb3->width,
            fb3->height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        fb3->bind();

        viewer.enable_skybox();
        viewer.draw_light_geom(light_geom_program);
      });

  auto hdr_program = std::make_shared<ShaderProgram>("hdr_shader");
  hdr_program->attach_vertex_shader("shaders/pass_pos_tex.vert");
  hdr_program->attach_fragment_shader("shaders/hdr_tone_mapping.frag");
  hdr_program->link();

  auto fb4 = std::make_shared<Framebuffer>(viewer.w.width, viewer.w.height, "hdr framebuffer");
  auto rp4 = std::make_shared<RenderPass>("hdr tone mapping pass", fb4, hdr_program, [&]() {
        fb3->bind_read();
        fb4->bind_draw();
        glBlitFramebuffer(0, 0, fb3->width, fb3->height, 0, 0, fb4->width,
            fb4->height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        static float exposure = 1.0;
        if (ImGui::BeginMainMenuBar()) {
          if (ImGui::BeginMenu("Lighting")) {
            ImGui::SliderFloat("exposure", &exposure, 0.0, 5.0);
            ImGui::EndMenu();
          }
          ImGui::EndMainMenuBar();
        }
        hdr_program->set_uniform("exposure", exposure);

        fb4->bind_draw();
        fb3->quad->draw(hdr_program);
      });


  viewer.add_render_pass(rp);
  viewer.add_render_pass(ssao_rp);
  viewer.add_render_pass(rp2);
  viewer.add_render_pass(rp3);
  viewer.add_render_pass(rp4);

  viewer.spawn_cube();
  viewer.render_loop_multipass([&] {});

  log_info("Render loop finished");
}

int main() {

  std::thread render_thread(test);

  // There appears to be a driver bug that causes a segfault on thread destruction
  render_thread.join();
}
