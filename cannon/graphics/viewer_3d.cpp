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
}
