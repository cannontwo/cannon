#include <cannon/graphics/viewer_3d.hpp>

using namespace cannon::graphics;

void Viewer3D::process_input_() {
  if (glfwGetKey(w.window, GLFW_KEY_W) == GLFW_PRESS)
    c.move_forward();
  if (glfwGetKey(w.window, GLFW_KEY_S) == GLFW_PRESS)
    c.move_backward();
  if (glfwGetKey(w.window, GLFW_KEY_A) == GLFW_PRESS)
    c.strafe_left();
  if (glfwGetKey(w.window, GLFW_KEY_D) == GLFW_PRESS)
    c.strafe_right();

  process_mouse_input_();
}

void Viewer3D::process_mouse_input_() {
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

  for (auto& g : scene_geom_) {
    g->draw(c.get_view_mat(), perspective);
  }

}

void Viewer3D::add_geom(std::shared_ptr<geometry::DrawableGeom> g) {
  scene_geom_.push_back(g);
}
