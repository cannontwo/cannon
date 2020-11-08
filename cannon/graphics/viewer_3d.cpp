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
  pitch_ += yoffset;

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

void Viewer3D::draw_scene_geom_() {
  Matrix4f perspective = make_perspective_fov(to_radians(45.0f),
      (float)(w.width) / (float)(w.height), 0.1f, 1000.0f);

  for (unsigned int i = 0; i < scene_geom_.size(); i++) {
    scene_geom_[i]->draw(c.get_view_mat(), perspective);
  }

  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("Geometry")) {
      for (unsigned int i = 0; i < scene_geom_.size(); i++) {
        scene_geom_[i]->write_imgui(i);
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}

void Viewer3D::add_geom(std::shared_ptr<geometry::DrawableGeom> g) {
  scene_geom_.push_back(g);
}

void Viewer3D::apply_light(std::shared_ptr<Light> l) {
  for (auto& g : scene_geom_) {
    l->apply(g);
  }
}

void Viewer3D::apply_light_collection(const LightCollection& l) {
  for (auto& g : scene_geom_) {
    l.apply(g);
  }
}
