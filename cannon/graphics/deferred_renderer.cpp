#include <cannon/graphics/deferred_renderer.hpp>

using namespace cannon::graphics;

void DeferredRenderer::render_loop(std::function<void()> f) {
  viewer.render_loop_multipass([&] {
      f();
      }, true);
}

void DeferredRenderer::setup_render_passes() {
  std::vector<std::string> faces;
  faces.push_back("assets/skybox/right.jpg");
  faces.push_back("assets/skybox/left.jpg");
  faces.push_back("assets/skybox/top.jpg");
  faces.push_back("assets/skybox/bottom.jpg");
  faces.push_back("assets/skybox/front.jpg");
  faces.push_back("assets/skybox/back.jpg");

  viewer.set_skybox(faces);
  viewer.w.set_clear_color({0.0, 0.0, 0.0, 1.0});

  std::vector<std::shared_ptr<Texture>> attachments;
  attachments.push_back(std::make_shared<Texture>(viewer.w.width,
        viewer.w.height, GL_RGBA32F, GL_FLOAT));
  attachments.push_back(std::make_shared<Texture>(viewer.w.width,
        viewer.w.height, GL_RGBA32F, GL_FLOAT));
  attachments.push_back(std::make_shared<Texture>(viewer.w.width,
        viewer.w.height, GL_RGBA32F, GL_FLOAT));

  gbuf_program_ = std::make_shared<ShaderProgram>("gbuffer_shader");
  gbuf_program_->attach_vertex_shader("shaders/mvp_normals_gbuffer.vert");
  gbuf_program_->attach_fragment_shader("shaders/gbuffer.frag");
  gbuf_program_->link();

  sdf_geom_program_ = std::make_shared<ShaderProgram>("sdf_geom_shader");
  sdf_geom_program_->attach_vertex_shader("shaders/sdf.vert");
  std::vector<std::string> libs = {"shaders/libs/sdf.glsl"};
  sdf_geom_program_->attach_fragment_shader("shaders/sdf.frag", libs);
  sdf_geom_program_->link();

  gbuffer_fb_ = viewer.add_render_pass("gbuffer",
      attachments, {gbuf_program_, sdf_geom_program_}, [this](){
            glClearColor(0.0, 0.0, 0.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            glDisable(GL_FRAMEBUFFER_SRGB); 
            viewer.disable_skybox();
            Vector3f c_pos = viewer.c.get_pos();
            Vector4f tmp_pos;
            tmp_pos << c_pos[0],
                       c_pos[1],
                       c_pos[2],
                       1.0;
            gbuf_program_->set_uniform("viewPos", tmp_pos);
            sdf_geom_program_->set_uniform("viewPos", tmp_pos);
            viewer.apply_light_collection(gbuf_program_);
            
            gbuffer_fb_->color_attachments[0]->unbind();

            viewer.draw_scene_geom(gbuf_program_, false);

            sdf_geom_program_->activate();
            sdf_geom_program_->set_uniform("time", (float)glfwGetTime());
            static int max_march_steps = 255;
            if (ImGui::BeginMainMenuBar()) {
              if (ImGui::BeginMenu("SDF")) {
                ImGui::SliderInt("max_march_steps", &max_march_steps, 0, 1000);
                ImGui::EndMenu();
              }
              ImGui::EndMainMenuBar();
            }
            sdf_geom_program_->set_uniform("max_march_steps", max_march_steps);
            viewer.draw_sdf_geom(sdf_geom_program_);
      });

    shadow_program_ = std::make_shared<ShaderProgram>("shadow_shader");
    shadow_program_->attach_vertex_shader("shaders/mvp_normals_tex.vert");
    shadow_program_->attach_fragment_shader("shaders/empty.frag");
    shadow_program_->link();

    sdf_shadow_program_ = std::make_shared<ShaderProgram>("sdf_shadow_shader");
    sdf_shadow_program_->attach_vertex_shader("shaders/sdf.vert");
    std::vector<std::string> shadow_libs = {"shaders/libs/sdf.glsl"};
    sdf_shadow_program_->attach_fragment_shader("shaders/sdf_shadow.frag", shadow_libs);
    sdf_shadow_program_->link();

    shadow_fb_ = viewer.add_render_pass("shadow", shadow_program_, [this](){
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // Set uniforms for light transform
        Camera c_backup = viewer.c;
        viewer.c = Camera({viewer.get_directional_light_pos()},
            {0.0, 0.0, 0.0}, {0.0, 1.0, 0.0});

        light_space_mat_ = make_orthographic(-20.0f, 20.0f, -20.0f, 20.0f, 1.0f,
            50.0f) * viewer.c.get_view_mat();

        Vector3f c_pos = viewer.c.get_pos();
        Vector4f tmp_pos;
        tmp_pos << c_pos[0],
                   c_pos[1],
                   c_pos[2],
                   1.0;
        shadow_program_->set_uniform("viewPos", tmp_pos);
        sdf_shadow_program_->set_uniform("viewPos", tmp_pos);

        shadow_fb_->color_attachments[0]->set_wrap_clamp_border();
        float borderColor[] = {1.0, 1.0, 1.0, 1.0};
        shadow_fb_->color_attachments[0]->bind();
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        viewer.draw_scene_geom(shadow_program_, false, true);

        sdf_shadow_program_->activate();
        sdf_shadow_program_->set_uniform("time", (float)glfwGetTime());
        static int max_march_steps = 255;
        if (ImGui::BeginMainMenuBar()) {
          if (ImGui::BeginMenu("SDF Shadow")) {
            ImGui::SliderInt("max_march_steps", &max_march_steps, 0, 1000);
            ImGui::EndMenu();
          }
          ImGui::EndMainMenuBar();
        }
        sdf_shadow_program_->set_uniform("max_march_steps", max_march_steps);
        viewer.draw_sdf_geom(sdf_shadow_program_, true);
        
        viewer.c = c_backup;
      });

  shadow_blur_program_ = std::make_shared<ShaderProgram>("shadow_blur_shader");
  shadow_blur_program_->attach_vertex_shader("shaders/pass_pos_tex.vert");
  shadow_blur_program_->attach_fragment_shader("shaders/shadow_blur.frag");
  shadow_blur_program_->link();

  shadow_blur_1_fb_ = viewer.add_render_pass("shadow hblur", shadow_blur_program_, [this](){
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        Vector4f tmp_dir;
        tmp_dir << 1.0, 0.0, 0.0, 0.0;
        shadow_blur_program_->set_uniform("direction", tmp_dir);

        shadow_blur_1_fb_->color_attachments[0]->set_wrap_clamp_border();
        float borderColor[] = {1.0, 1.0, 1.0, 1.0};
        shadow_blur_1_fb_->color_attachments[0]->bind();
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        shadow_fb_->bind_read();
        shadow_blur_1_fb_->bind_draw();
        shadow_fb_->quad->draw(shadow_blur_program_);
      });

  shadow_blur_2_fb_ = viewer.add_render_pass("shadow vblur", shadow_blur_program_, [this](){
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        Vector4f tmp_dir;
        tmp_dir << 0.0, 1.0, 0.0, 0.0;
        shadow_blur_program_->set_uniform("direction", tmp_dir);

        shadow_blur_2_fb_->color_attachments[0]->set_wrap_clamp_border();
        float borderColor[] = {1.0, 1.0, 1.0, 1.0};
        shadow_blur_2_fb_->color_attachments[0]->bind();
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        shadow_blur_1_fb_->bind_read();
        shadow_blur_2_fb_->bind_draw();
        shadow_blur_1_fb_->quad->draw(shadow_blur_program_);
      });

  std::mt19937 gen;
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

    ssao_kernel_.push_back(tmp);
  }

  for (int i = 0; i < 16; i++) {
    float t1 = d(gen) * 2.0 - 1.0;
    float t2 = d(gen) * 2.0 - 1.0;

    ssao_noise_.push_back(t1);
    ssao_noise_.push_back(t2);
    ssao_noise_.push_back(0.0);
    ssao_noise_.push_back(1.0);
  }


  auto noise_tex = std::make_shared<Texture>(4, 4, GL_RGBA32F, GL_FLOAT, GL_RGBA, ssao_noise_.data());
  noise_tex->set_filter_nearest();
  noise_tex->set_wrap_repeat();

  ssao_program_ = std::make_shared<ShaderProgram>("ssao_shader");
  ssao_program_->attach_vertex_shader("shaders/pass_pos_tex.vert");
  ssao_program_->attach_fragment_shader("shaders/ssao.frag");
  ssao_program_->link();

  ssao_fb_ = viewer.add_render_pass("ssao", ssao_program_, [this, noise_tex](){
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        Vector3f c_pos = viewer.c.get_pos();
        Vector4f tmp_pos;
        tmp_pos << c_pos[0],
                   c_pos[1],
                   c_pos[2],
                   1.0;
        ssao_program_->set_uniform("viewPos", tmp_pos);
      
        ssao_program_->set_uniform("gPosition", 0);
        ssao_program_->set_uniform("gNormal", 1);
        ssao_program_->set_uniform("texNoise", 2);

        ssao_program_->set_uniform("screen_width", (float)viewer.w.width);
        ssao_program_->set_uniform("screen_height", (float)viewer.w.height);

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
        ssao_program_->set_uniform("radius", radius);
        ssao_program_->set_uniform("bias", bias);

        // Set samples uniform
        for (int i = 0; i < 64; i++)
          ssao_program_->set_uniform("samples[" + std::to_string(i) + "]", (Vector4f)ssao_kernel_[i]);

        Matrix4f perspective = make_perspective_fov(to_radians(45.0f),
            (float)(viewer.w.width) / (float)(viewer.w.height), 0.1f, 50.0f);

        ssao_program_->set_uniform("projection", perspective);
        ssao_program_->set_uniform("view", viewer.c.get_view_mat());

        std::vector<std::shared_ptr<Texture>> ssao_attachments;
        ssao_attachments.push_back(gbuffer_fb_->color_attachments[0]);
        ssao_attachments.push_back(gbuffer_fb_->color_attachments[1]);
        ssao_attachments.push_back(noise_tex);

        ssao_fb_->quad->draw(ssao_program_, ssao_attachments);
      });

  ssao_blur_program_ = std::make_shared<ShaderProgram>("ssao_blur_shader");
  ssao_blur_program_->attach_vertex_shader("shaders/pass_pos_tex.vert");
  ssao_blur_program_->attach_fragment_shader("shaders/ssao_blur.frag");
  ssao_blur_program_->link();

  ssao_blur_fb_ = viewer.add_render_pass("ssao blur",
      ssao_blur_program_, [this](){
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        ssao_blur_fb_->bind_read();
        ssao_blur_fb_->bind_draw();
        ssao_fb_->quad->draw(ssao_blur_program_);
      });

  lighting_program_ = std::make_shared<ShaderProgram>("lighting_shader");
  lighting_program_->attach_vertex_shader("shaders/pass_pos_tex.vert");
  lighting_program_->attach_fragment_shader("shaders/deferred_lighting.frag");
  lighting_program_->link();

  lighting_fb_ = viewer.add_render_pass("lighting", lighting_program_, [this](){
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        Vector3f c_pos = viewer.c.get_pos();
        Vector4f tmp_pos;
        tmp_pos << c_pos[0],
                   c_pos[1],
                   c_pos[2],
                   1.0;
        lighting_program_->set_uniform("viewPos", tmp_pos);
        lighting_program_->set_uniform("shininess", (float)32.0);
        viewer.apply_light_collection(lighting_program_);

        lighting_program_->set_uniform("gPosition", 0);
        lighting_program_->set_uniform("gNormal", 1);
        lighting_program_->set_uniform("gAlbedoSpec", 2);
        lighting_program_->set_uniform("ssao", 3);
        lighting_program_->set_uniform("shadowMap", 4);

        lighting_program_->set_uniform("lightSpaceMatrix", light_space_mat_);

        static bool enable_ssao = true;
        if (ImGui::BeginMainMenuBar()) {
          if (ImGui::BeginMenu("SSAO")) {
            ImGui::Checkbox("enable", &enable_ssao);
            ImGui::EndMenu();
          }
          ImGui::EndMainMenuBar();
        }

        lighting_program_->set_uniform("enable_ssao", enable_ssao);

        std::vector<std::shared_ptr<Texture>> lighting_attachments;
        lighting_attachments.push_back(gbuffer_fb_->color_attachments[0]);
        lighting_attachments.push_back(gbuffer_fb_->color_attachments[1]);
        lighting_attachments.push_back(gbuffer_fb_->color_attachments[2]);
        lighting_attachments.push_back(ssao_blur_fb_->color_attachments[0]);
        lighting_attachments.push_back(shadow_blur_2_fb_->color_attachments[0]);

        lighting_fb_->quad->draw(lighting_program_, lighting_attachments);
      });


  light_geom_program_ = std::make_shared<ShaderProgram>("light_geom_shader");
  light_geom_program_->attach_vertex_shader("shaders/mvp_uniform_color.vert");
  light_geom_program_->attach_fragment_shader("shaders/pass_color.frag");
  light_geom_program_->link();

  light_geom_fb_ = viewer.add_render_pass("light geom", light_geom_program_, [this]() {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        Vector3f c_pos = viewer.c.get_pos();
        Vector4f tmp_pos;
        tmp_pos << c_pos[0],
                   c_pos[1],
                   c_pos[2],
                   1.0;
        light_geom_program_->set_uniform("uColor", Vector4f{1.0, 1.0, 1.0, 1.0});
        viewer.apply_light_collection(light_geom_program_);

        lighting_fb_->bind_read();
        light_geom_fb_->bind_draw();
        glBlitFramebuffer(0, 0, lighting_fb_->width, lighting_fb_->height, 0,
            0, light_geom_fb_->width, light_geom_fb_->height,
            GL_COLOR_BUFFER_BIT, GL_NEAREST);
        light_geom_fb_->bind();

        gbuffer_fb_->bind_read();
        light_geom_fb_->bind_draw();
        glBlitFramebuffer(0, 0, gbuffer_fb_->width, gbuffer_fb_->height, 0, 0, light_geom_fb_->width,
            light_geom_fb_->height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        light_geom_fb_->bind();

        viewer.enable_skybox();
        viewer.draw_light_geom(light_geom_program_);
      });

  hdr_program_ = std::make_shared<ShaderProgram>("hdr_shader");
  hdr_program_->attach_vertex_shader("shaders/pass_pos_tex.vert",{"shaders/libs/sdf.glsl",
      "shaders/libs/foo.glsl"});
  hdr_program_->attach_fragment_shader("shaders/hdr_tone_mapping.frag");
  hdr_program_->link();

  hdr_fb_ = viewer.add_render_pass("hdr tone mapping", hdr_program_, [this]() {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        static float exposure = 0.0;
        if (ImGui::BeginMainMenuBar()) {
          if (ImGui::BeginMenu("Lighting")) {
            ImGui::SliderFloat("exposure", &exposure, -4.0, 4.0);
            ImGui::EndMenu();
          }
          ImGui::EndMainMenuBar();
        }
        hdr_program_->set_uniform("exposure", exposure);

        light_geom_fb_->bind_read();
        hdr_fb_->bind_draw();
        light_geom_fb_->quad->draw(hdr_program_);
      });

  axes_program_ = std::make_shared<ShaderProgram>("axes_shader");
  axes_program_->attach_vertex_shader("shaders/pass_pos.vert");
  axes_program_->attach_fragment_shader("shaders/axes_hint.frag");
  axes_program_->link();


  axes_fb_ = viewer.add_render_pass("axes hint", axes_program_, [this]() {
      glClearColor(0.0, 0.0, 0.0, 1.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
      hdr_fb_->bind_read();
      axes_fb_->bind_draw();
      hdr_fb_->quad->draw();

      axes_fb_->bind_read();
      Matrix4f perspective = make_orthographic(-1.0, 1.0, -1.0, 1.0, -2.0,
          2.0);

      // Rotation-only lookAt matrix
      Vector3f tmp_pos = -viewer.c.get_direction().normalized();
      Vector3f tmp_dir = viewer.c.get_direction().normalized();
      Vector3f tmp_up = tmp_dir.cross(viewer.c.get_right());
      Vector3f right = viewer.c.get_right();

      Matrix4f rot_mat;
      Matrix4f trans_mat;

      rot_mat << right[0], right[1], right[2], 0.0,
                 tmp_up[0], tmp_up[1], tmp_up[2], 0.0,
                 tmp_dir[0], tmp_dir[1], tmp_dir[2], 0.0,
                 0.0, 0.0, 0.0, 1.0;

      trans_mat << 1.0, 0.0, 0.0, -tmp_pos[0],
                   0.0, 1.0, 0.0, -tmp_pos[1],
                   0.0, 0.0, 1.0, -tmp_pos[2],
                   0.0, 0.0, 0.0, 1.0;

      Matrix4f view = rot_mat * trans_mat;

      glDisable(GL_DEPTH_TEST);
      int start_x = viewer.w.width - 150;
      int start_y = viewer.w.height - 150;
      int small_width = 100;
      int small_height = 100;
      glViewport(start_x, start_y, small_width, small_height);
      glLineWidth(5.0);
      ah_.draw(axes_program_, view, perspective);
      glEnable(GL_DEPTH_TEST);

      glViewport(0, 0, viewer.w.width, viewer.w.height);
      });
}


float cannon::graphics::lerp(float a, float b, float f) {
  return a + f * (b - a);
}
