#include <cannon/graphics/viewer_3d.hpp>

using namespace cannon::graphics;

void Viewer3D::draw() {
  write_imgui(true);
  lc_.write_imgui();

  for (auto &pass : render_passes_) {
    pass->run();
    pass->write_imgui();
  }
}

void Viewer3D::draw_scene_geom(bool draw_lights, bool ortho) {
  Matrix4f perspective;
  if (ortho) {
    perspective = make_orthographic(-20.0f, 20.0f, -20.0f, 20.0f, 1.0f, 50.0f);
  } else {
  perspective = make_perspective_fov(to_radians(45.0f),
      (float)(w.width) / (float)(w.height), 0.1f, 50.0f);
  }

  for (unsigned int i = 0; i < scene_geom_.size(); i++) {
    scene_geom_[i]->draw(c.get_view_mat(), perspective);
  }

  if (draw_lights) {
    for (unsigned int i = 0; i < light_geom_.size(); i++) {
      light_geom_[i]->draw(c.get_view_mat(), perspective);
    }
  }

  if (draw_skybox_) {
    skybox_->draw(c.get_view_mat(), perspective);
  }
}

void Viewer3D::draw_scene_geom(std::shared_ptr<ShaderProgram> p, bool draw_lights, bool ortho) {
  Matrix4f perspective;
  if (ortho) {
    perspective = make_orthographic(-20.0f, 20.0f, -20.0f, 20.0f, 1.0f, 50.0f);
  } else {
  perspective = make_perspective_fov(to_radians(45.0f),
      (float)(w.width) / (float)(w.height), 0.1f, 50.0f);
  }

  for (unsigned int i = 0; i < scene_geom_.size(); i++) {
    scene_geom_[i]->draw(p, c.get_view_mat(), perspective);
  }

  if (draw_lights) {
    for (unsigned int i = 0; i < light_geom_.size(); i++) {
      light_geom_[i]->draw(c.get_view_mat(), perspective);
    }
  }

  if (draw_skybox_) {
    skybox_->draw(c.get_view_mat(), perspective);
  }
}

void Viewer3D::draw_light_geom() {
  Matrix4f perspective = make_perspective_fov(to_radians(45.0f),
      (float)(w.width) / (float)(w.height), 0.1f, 50.0f);

  for (unsigned int i = 0; i < light_geom_.size(); i++) {
    light_geom_[i]->draw(c.get_view_mat(), perspective);
  }

  if (draw_skybox_) {
    skybox_->draw(c.get_view_mat(), perspective);
  }
}

void Viewer3D::draw_light_geom(std::shared_ptr<ShaderProgram> p) {
  Matrix4f perspective = make_perspective_fov(to_radians(45.0f),
      (float)(w.width) / (float)(w.height), 0.1f, 50.0f);

  for (unsigned int i = 0; i < light_geom_.size(); i++) {
    light_geom_[i]->draw(p, c.get_view_mat(), perspective);
  }

  if (draw_skybox_) {
    skybox_->draw(c.get_view_mat(), perspective);
  }
}

void Viewer3D::draw_sdf_geom(bool ortho) {
  Matrix4f perspective;
  if (ortho) {
    perspective = make_orthographic(-20.0f, 20.0f, -20.0f, 20.0f, 1.0f, 50.0f);
  } else {
  perspective = make_perspective_fov(to_radians(45.0f),
      (float)(w.width) / (float)(w.height), 0.1f, 50.0f);
  }

  for (unsigned int i = 0; i < sdf_geom_.size(); i++) {
    sdf_geom_[i]->draw(c.get_view_mat(), perspective);
  }
}

void Viewer3D::draw_sdf_geom(std::shared_ptr<ShaderProgram> p, bool ortho) {
  Matrix4f perspective;
  if (ortho) {
    perspective = make_orthographic(-20.0f, 20.0f, -20.0f, 20.0f, 1.0f, 50.0f);
  } else {
  perspective = make_perspective_fov(to_radians(45.0f),
      (float)(w.width) / (float)(w.height), 0.1f, 50.0f);
  }

  for (unsigned int i = 0; i < sdf_geom_.size(); i++) {
    sdf_geom_[i]->draw(p, c.get_view_mat(), perspective);
  }
}

void Viewer3D::add_geom(std::shared_ptr<geometry::DrawableGeom> g) {
  scene_geom_.push_back(g);
  add_shader(g->program);
}

void Viewer3D::add_shader(std::shared_ptr<ShaderProgram> s) {
  if (std::find(shaders_.begin(), shaders_.end(), s) == shaders_.end()) {
    shaders_.push_back(s);
  }
}

void Viewer3D::apply_light(std::shared_ptr<Light> l) {
  for (auto& g : scene_geom_) {
    l->apply(g);
  }
}

void Viewer3D::apply_light_collection(const LightCollection& l) {
  // In the case of a large number of lights, we expect this to be a speed up
  for (auto& s : shaders_) {
    l.apply(s);
  }
}

void Viewer3D::apply_light_collection(std::shared_ptr<ShaderProgram> p) {
  lc_.apply(p);
}

void Viewer3D::set_skybox(std::vector<std::string> face_paths) {
  draw_skybox_ = true;
  skybox_ = std::make_shared<geometry::Skybox>(face_paths);
  add_shader(skybox_->program);
}

void Viewer3D::enable_skybox() {
  draw_skybox_ = true;
}

void Viewer3D::disable_skybox() {
  draw_skybox_ = false;
}

Vector3f Viewer3D::get_directional_light_pos() {
  return -20.0 * lc_.get_directional_light()->get_dir();
}

Vector3f Viewer3D::get_directional_light_dir() {
  return lc_.get_directional_light()->get_dir();
}

std::shared_ptr<geometry::Cube> Viewer3D::spawn_cube() {
  auto cube = std::make_shared<geometry::Cube>(geom_program_);
  add_geom(cube);

  Vector3f pos1 = c.get_pos() - 2.0 * c.get_direction().normalized();

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dist(-M_PI, M_PI);

  AngleAxisf rot1(dist(gen), Vector3f::Random().normalized());

  cube->set_pos(pos1);
  cube->set_rot(rot1);

  return cube;
}

std::shared_ptr<geometry::Plane> Viewer3D::spawn_plane() {
  auto plane = std::make_shared<geometry::Plane>(geom_program_);
  add_geom(plane);

  Vector3f pos1 = c.get_pos() - 2.0 * c.get_direction().normalized();

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dist(-M_PI, M_PI);

  AngleAxisf rot1(dist(gen), Vector3f::Random().normalized());

  plane->set_pos(pos1);
  plane->set_rot(rot1);

  return plane;
}

std::shared_ptr<geometry::Model> Viewer3D::spawn_model(const std::string& path) {
  auto m = std::make_shared<geometry::Model>(geom_program_, path);
  add_geom(m);

  Vector3f pos1 = c.get_pos() - 2.0 * c.get_direction().normalized();

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dist(-M_PI, M_PI);

  AngleAxisf rot1(dist(gen), Vector3f::Random().normalized());

  m->set_pos(pos1);
  m->set_rot(rot1);

  return m;
}

void Viewer3D::spawn_point_light() {
  Vector4f light_color;
  light_color << 1.0,
                 1.0,
                 1.0,
                 1.0;

  geometry::Material light_material(light_color*5, {0.0, 0.0, 0.0, 1.0},
      {0.0, 0.0, 0.0, 1.0}, 32.0);

  auto light_cube = std::make_shared<geometry::Cube>(geom_program_);
  auto light = std::make_shared<PointLight>(light_color*0.2, light_color*0.5, light_color, light_cube);
  light_geom_.push_back(light_cube);

  Vector4f light_pos;
  Vector3f pos1 = c.get_pos() - 2.0 * c.get_direction().normalized();
  light_pos.head(3) = pos1;
  light_pos[3] = 1.0;
  light->set_pos(light_pos);

  light_cube->set_material(light_material);
  light_cube->set_pos(light_pos.head(3));
  light_cube->set_scale(0.2);

  // Lights rendered saturated
  light->set_ambient(light_color);
  light->apply(light_cube);
  light->set_ambient(light_color * 0.2);

  lc_.add_light(light);
}

void Viewer3D::spawn_spotlight() {
  Vector4f light_color;
  light_color << 1.0,
                 1.0,
                 1.0,
                 1.0;

  geometry::Material light_material(light_color, {0.0, 0.0, 0.0, 1.0},
      {0.0, 0.0, 0.0, 1.0}, 32.0);

  Vector4f light_dir;
  light_dir.head(3) = -c.get_direction();
  light_dir[3] = 1.0;
  auto light = std::make_shared<Spotlight>(light_color*0.2,
      light_color*0.5, light_color, light_dir);

  Vector4f light_pos;
  Vector3f pos1 = c.get_pos() - 2.0 * c.get_direction().normalized();
  light_pos.head(3) = pos1;
  light_pos[3] = 1.0;
  light->set_pos(light_pos);

  lc_.add_light(light);
}

void Viewer3D::spawn_sdf_volume() {
  auto vol = std::make_shared<geometry::SDFVolume>(sdf_program_);
  sdf_geom_.push_back(vol);

  Vector3f pos1 = c.get_pos() - 2.0 * c.get_direction().normalized();

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<float> dist(-M_PI, M_PI);

  AngleAxisf rot1(dist(gen), Vector3f::Random().normalized());

  vol->set_pos(pos1);
  vol->set_rot(rot1);
}

void Viewer3D::write_imgui(bool multipass) {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("Spawn")) {
      if (ImGui::BeginMenu("Geometry")) {
        if (ImGui::Button("Cube")) {
          spawn_cube();
        }

        if (ImGui::Button("Plane")) {
          spawn_plane();
        }

        if (ImGui::Button("SDF Volume")) {
          spawn_sdf_volume();
        }

        if (ImGui::BeginMenu("Model")) {
          static std::string model_path("assets/backpack/backpack.obj");
          ImGui::InputText("Model path", &model_path);
          if (ImGui::Button("Spawn")) {
            try {
              spawn_model(model_path);
            } catch (...) {
              log_warning("Could not load model at", model_path);
            }
          }
          ImGui::EndMenu();
        }

        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Lights")) {
        if (ImGui::Button("Point Light")) {
          spawn_point_light();        
        }

        if (ImGui::Button("Spotlight")) {
          spawn_spotlight();
        }
        ImGui::EndMenu();
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Geometry")) {
      for (unsigned int i = 0; i < scene_geom_.size(); i++) {
        scene_geom_[i]->write_imgui(i);
      }

      for (unsigned int i = 0; i < sdf_geom_.size(); i++) {
        sdf_geom_[i]->write_imgui(i);
      }
      ImGui::EndMenu();
    }

    if (!multipass) {
      if (ImGui::BeginMenu("Shaders")) {
        for (auto &s : shaders_) {
          s->write_imgui();
        }
        ImGui::EndMenu();
      }
    }

    if (ImGui::BeginMenu("Textures")) {
      for (auto &t : Texture::get_loaded_textures()) {
        t->write_imgui();
      }
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }
}

void Viewer3D::add_render_pass(std::shared_ptr<RenderPass> rp) {
  render_passes_.push_back(rp);  

  // By convention, initial geometry rendering happens on the first render
  // pass, and drawing to screen happens on the last
  w.render_to_framebuffer(render_passes_.front()->framebuffer);
  w.draw_from_framebuffer(render_passes_.back()->framebuffer);
}

std::shared_ptr<Framebuffer> Viewer3D::add_render_pass(const std::string& name,
    std::shared_ptr<ShaderProgram> p, std::function<void()> f) {
  auto fb = std::make_shared<Framebuffer>(w.width, w.height, name + " framebuffer");
  auto rp = std::make_shared<RenderPass>(name + " pass", fb, p, f);

  render_passes_.push_back(rp);  
  w.render_to_framebuffer(render_passes_.front()->framebuffer);
  w.draw_from_framebuffer(render_passes_.back()->framebuffer);

  return fb;
}

std::shared_ptr<Framebuffer> Viewer3D::add_render_pass(const std::string& name,
    std::vector<std::shared_ptr<Texture>> attachments,
    std::vector<std::shared_ptr<ShaderProgram>> programs, std::function<void()> f) {
  auto fb = std::make_shared<Framebuffer>(attachments, w.width, w.height, name + " framebuffer");
  auto rp = std::make_shared<RenderPass>(name + " pass", fb, programs, f);

  render_passes_.push_back(rp);  
  w.render_to_framebuffer(render_passes_.front()->framebuffer);
  w.draw_from_framebuffer(render_passes_.back()->framebuffer);

  return fb;
}

void Viewer3D::initialize_lc_() {
  Vector4f light_color;
  light_color << 0.5,
                 0.5,
                 0.5,
                 0.5;
  auto dl = std::make_shared<DirectionalLight>(light_color*0.2, light_color*0.5, light_color);
  dl->set_direction({-0.2, -1.0, -0.3, -1.0});
  lc_.add_light(dl);
}

void Viewer3D::make_shaders_() {
  geom_program_ = std::make_shared<ShaderProgram>("geom_shader");
  geom_program_->attach_vertex_shader("shaders/mvp_normals_tex.vert");
  geom_program_->attach_fragment_shader("shaders/material_lights_tex.frag");
  geom_program_->link();

  sdf_program_ = std::make_shared<ShaderProgram>("sdf_shader");
  sdf_program_->attach_vertex_shader("shaders/sdf.vert");
  std::vector<std::string> libs = {"shaders/libs/sdf.glsl"};
  sdf_program_->attach_fragment_shader("shaders/sdf.frag", libs);
  sdf_program_->link();
}

void Viewer3D::set_callbacks_() {
  glfwSetWindowUserPointer(w.window, this);
  glfwSetDropCallback(w.window, drop_callback);
  glfwSetKeyCallback(w.window, viewer_key_callback);
  glfwSetFramebufferSizeCallback(w.window, viewer3d_framebuffer_size_callback);
  glfwSetMouseButtonCallback(w.window, mouse_button_callback);
  glfwSetCursorPosCallback(w.window, cursor_pos_callback);
}

void Viewer3D::populate_initial_geometry_() {
  auto plane = std::make_shared<geometry::Plane>(geom_program_);
  add_geom(plane);

  Vector3f pos;
  pos << 0.0, -5.0, 0.0;

  AngleAxisf rot(to_radians(-90), Vector3f::UnitX());

  plane->set_pos(pos);
  plane->set_rot(rot);
  plane->set_scale(20.0);

  auto plane2 = std::make_shared<geometry::Plane>(geom_program_);
  add_geom(plane2);

  Vector3f pos2;
  pos2 << 10.0, 0.0, 0.0;

  AngleAxisf rot2(to_radians(-90), Vector3f::UnitY());

  plane2->set_pos(pos2);
  plane2->set_rot(rot2);
  plane2->set_scale(20.0);

  auto plane3 = std::make_shared<geometry::Plane>(geom_program_);

  Vector3f pos3;
  pos3 << -10.0, 0.0, 0.0;

  AngleAxisf rot3(to_radians(90), Vector3f::UnitY());

  plane3->set_pos(pos3);
  plane3->set_rot(rot3);
  plane3->set_scale(20.0);

  add_geom(plane3);

  auto plane4 = std::make_shared<geometry::Plane>(geom_program_);
  Vector3f pos4;
  pos4 << 0.0, 0.0, -10.0;

  AngleAxisf rot4(to_radians(0), Vector3f::UnitZ());

  plane4->set_pos(pos4);
  plane4->set_rot(rot4);
  plane4->set_scale(20.0);

  add_geom(plane4);

  auto plane5 = std::make_shared<geometry::Plane>(geom_program_);
  Vector3f pos5;
  pos5 << 0.0, 0.0, 10.0;

  AngleAxisf rot5(to_radians(180), Vector3f::UnitY());

  plane5->set_pos(pos5);
  plane5->set_rot(rot5);
  plane5->set_scale(20.0);

  add_geom(plane5);
}

// Callbacks
void cannon::graphics::drop_callback(GLFWwindow *window, int path_count, const char* paths[]) {
  Viewer3D *viewer = (Viewer3D*)glfwGetWindowUserPointer(window);

  // Assuming that only model paths are dropped
  for (int i = 0; i < path_count; i++) {
    std::string path(paths[i]);
    try {
      viewer->spawn_model(path);
    } catch (...) {
      log_warning("Could not load model at", path);
    }
  }
}

void cannon::graphics::viewer3d_framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);

  Viewer3D *viewer = (Viewer3D*)glfwGetWindowUserPointer(window);
  viewer->w.width = width;
  viewer->w.height = height;
  viewer->w.set_viewport(0, 0, width, height);

  for (auto& pass : viewer->render_passes_) {
    pass->framebuffer->resize(width, height);
  }

  // For smooth resizing
  viewer->w.draw([&]() {viewer->draw();});
}

void cannon::graphics::viewer_key_callback(GLFWwindow* window, int key, int
    scancode, int action, int mods) {
  Viewer3D *viewer = (Viewer3D*)glfwGetWindowUserPointer(window);

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
    viewer->c.set_speed(2.0);
  if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
    viewer->c.set_speed(1.0);

  if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
    viewer->c.set_move_forward(true);
  if (key == GLFW_KEY_W && action == GLFW_RELEASE)
    viewer->c.set_move_forward(false);

  if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
    viewer->c.set_move_backward(true);
  if (key == GLFW_KEY_S && action == GLFW_RELEASE)
    viewer->c.set_move_backward(false);

  if (key == GLFW_KEY_A && action == (GLFW_PRESS || action == GLFW_REPEAT))
    viewer->c.set_strafe_left(true);
  if (key == GLFW_KEY_A && action == GLFW_RELEASE)
    viewer->c.set_strafe_left(false);

  if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
    viewer->c.set_strafe_right(true);
  if (key == GLFW_KEY_D && action == GLFW_RELEASE)
    viewer->c.set_strafe_right(false);
}

void cannon::graphics::mouse_button_callback(GLFWwindow* window, int button,
    int action, int mods) {
  Viewer3D *viewer = (Viewer3D*)glfwGetWindowUserPointer(window);

  if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
    if (!viewer->mouse_captured_) {
      viewer->mouse_captured_ = true;
      viewer->first_mouse_ = true;
      glfwSetInputMode(viewer->w.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } 
  }

  if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE) {
    if (viewer->mouse_captured_) {
      viewer->mouse_captured_ = false;
      glfwSetInputMode(viewer->w.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } 
  }
}

void cannon::graphics::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
  Viewer3D *viewer = (Viewer3D*)glfwGetWindowUserPointer(window);

  if (!viewer->mouse_captured_)
    return;

  if (viewer->first_mouse_) {
    viewer->last_x_ = xpos;
    viewer->last_y_ = ypos;
    viewer->first_mouse_ = false;
  }

  float xoffset = (float)xpos - viewer->last_x_;
  float yoffset = viewer->last_y_ - (float)ypos; // reversed due to screen coords
  viewer->last_x_ = xpos;
  viewer->last_y_ = ypos;

  xoffset *= viewer->mouse_sensitivity_;
  yoffset *= viewer->mouse_sensitivity_; 

  viewer->yaw_ += xoffset;
  viewer->pitch_ -= yoffset;

  if (viewer->pitch_ > (M_PI / 2.1))
    viewer->pitch_ = M_PI / 2.1;
  if (viewer->pitch_ < (-M_PI / 2.1))
    viewer->pitch_ = -M_PI / 2.1;

  Vector3f new_dir;
  new_dir << std::cos(viewer->yaw_) * std::cos(viewer->pitch_),
             std::sin(viewer->pitch_),
             std::sin(viewer->yaw_) * std::cos(viewer->pitch_);
  new_dir.normalize();
  viewer->c.set_direction(new_dir);
}
