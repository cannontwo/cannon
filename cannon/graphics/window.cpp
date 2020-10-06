#include <cannon/graphics/window.hpp>

using namespace cannon::graphics;
using namespace cannon::log;

void Window::make_current() {
  glfwMakeContextCurrent(window);
}

void Window::set_viewport(unsigned x, unsigned y, unsigned width, unsigned height) {
  glViewport(x, y, width, height);
}

void Window::register_callbacks() {
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glDebugMessageCallback((GLDEBUGPROC)debug_message_callback, (void *)nullptr);
}

void Window::set_clear_color(Vector4f color) {
  clear_color_ = color;
}

void Window::set_wireframe_mode() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Window::set_fill_mode() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// Free functions
void cannon::graphics::init_glfw() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
}

void cannon::graphics::init_glad() {
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    throw std::runtime_error("Could not initialize glad");
  }
}

void Window::enable_depth_test() {
  glEnable(GL_DEPTH_TEST);
}

void Window::disable_depth_test() {
  glDisable(GL_DEPTH_TEST);
}

void Window::save_image(const std::string &path) {
  int num_channels = 3;
  int stride = num_channels * width;
  stride += (stride % 4) ? (4 - stride % 4) : 0; // Alignment

  int buffer_size = stride * height;
  std::vector<char> buffer(buffer_size);

  // Configure reading
  glPixelStorei(GL_PACK_ALIGNMENT, 4);
  glReadBuffer(GL_FRONT);

  // Read into buffer
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());

  // Write to file
  stbi_flip_vertically_on_write(true);
  if (stbi_write_png(path.c_str(), width, height, num_channels, buffer.data(), stride) == 0) {
    throw std::runtime_error("Could not write OpenGL render to file");
  }
}

// Callbacks
void cannon::graphics::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void cannon::graphics::process_input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

void cannon::graphics::debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum
    severity, GLsizei length, const GLchar *message, 
    const void *userParam) {
  log_warning(message);
}
