#include <cannon/graphics/viewer_3d.hpp>

using namespace cannon::graphics;

void Viewer3D::process_input_() {
  if (glfwGetKey(w.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    c.set_speed(0.2);
  if (glfwGetKey(w.window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    c.set_speed(0.05);

  if (glfwGetKey(w.window, GLFW_KEY_W) == GLFW_PRESS)
    c.move_forward();
  if (glfwGetKey(w.window, GLFW_KEY_S) == GLFW_PRESS)
    c.move_backward();
  if (glfwGetKey(w.window, GLFW_KEY_A) == GLFW_PRESS)
    c.strafe_left();
  if (glfwGetKey(w.window, GLFW_KEY_D) == GLFW_PRESS)
    c.strafe_right();

  if (glfwGetMouseButton(w.window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
    if (!mouse_captured_) {
      mouse_captured_ = true;
      first_mouse_ = true;
      glfwSetInputMode(w.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } 
  }

  if (glfwGetMouseButton(w.window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_RELEASE) {
    if (mouse_captured_) {
      mouse_captured_ = false;
      glfwSetInputMode(w.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } 
  }

  process_mouse_input_();
}

void Viewer3D::process_mouse_input_() {
  if (!mouse_captured_)
    return;

  double xpos, ypos;
  glfwGetCursorPos(w.window, &xpos, &ypos);
  if (first_mouse_) {
    last_x_ = xpos;
    last_y_ = ypos;
    first_mouse_ = false;
  }

  float xoffset = (float)xpos - last_x_;
  float yoffset = last_y_ - (float)ypos; // reversed due to screen coords
  last_x_ = xpos;
  last_y_ = ypos;

  xoffset *= mouse_sensitivity_;
  yoffset *= mouse_sensitivity_; 

  yaw_ += xoffset;
  pitch_ -= yoffset;

  if (pitch_ > (M_PI / 2.1))
    pitch_ = M_PI / 2.1;
  if (pitch_ < (-M_PI / 2.1))
    pitch_ = -M_PI / 2.1;

  Vector3f new_dir;
  new_dir << std::cos(yaw_) * std::cos(pitch_),
             std::sin(pitch_),
             std::sin(yaw_) * std::cos(pitch_);
  new_dir.normalize();
  c.set_direction(new_dir);
}

void Viewer3D::draw_scene_geom(bool draw_lights) {
  Matrix4f perspective = make_perspective_fov(to_radians(45.0f),
      (float)(w.width) / (float)(w.height), 0.1f, 1000.0f);

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

void Viewer3D::draw_scene_geom(std::shared_ptr<ShaderProgram> p, bool draw_lights) {
  Matrix4f perspective = make_perspective_fov(to_radians(45.0f),
      (float)(w.width) / (float)(w.height), 0.1f, 1000.0f);

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
      (float)(w.width) / (float)(w.height), 0.1f, 1000.0f);

  for (unsigned int i = 0; i < light_geom_.size(); i++) {
    light_geom_[i]->draw(c.get_view_mat(), perspective);
  }

  if (draw_skybox_) {
    skybox_->draw(c.get_view_mat(), perspective);
  }
}

void Viewer3D::draw_light_geom(std::shared_ptr<ShaderProgram> p) {
  Matrix4f perspective = make_perspective_fov(to_radians(45.0f),
      (float)(w.width) / (float)(w.height), 0.1f, 1000.0f);

  for (unsigned int i = 0; i < light_geom_.size(); i++) {
    light_geom_[i]->draw(p, c.get_view_mat(), perspective);
  }

  if (draw_skybox_) {
    skybox_->draw(c.get_view_mat(), perspective);
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
}

void Viewer3D::set_callbacks_() {
  glfwSetWindowUserPointer(w.window, this);
  glfwSetDropCallback(w.window, drop_callback);
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
