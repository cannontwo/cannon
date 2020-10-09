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

void Window::set_text_color(Vector4f color) {
  text_color_ = color;
}

void Window::set_wireframe_mode() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Window::set_fill_mode() {
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Window::display_text(float x, float y, float scale, const std::string &text) {
  overlays_.push_back({x, y, scale, text});
}

void Window::init_text_shader() {
  VertexShader v = load_vertex_shader("shaders/char_shader.vert");
  FragmentShader f = load_fragment_shader("shaders/char_shader.frag");

  text_program_.attach_shader(v);
  text_program_.attach_shader(f);
  text_program_.link();
  text_program_.activate();

  // Reserve space for display quads
  MatrixX4f vertices(MatrixX4f::Zero(6, 4));
  buf_.bind();
  buf_.buffer(vertices);
}

void Window::draw_overlays() {
  text_program_.activate();
  vao_->bind();
  text_program_.set_uniform("projection", make_orthographic(0.0f, (float)width,
        0.0f, (float)height, -1.0, 1.0));
  text_program_.set_uniform("textColor", text_color_);

  for (auto& o : overlays_) {
    float x = o.x;
    std::string::const_iterator c;   

    for (c = o.text.begin(); c != o.text.end(); c++) {
      std::shared_ptr<Character> ch = font_.get_char(*c);

      float xpos = x + ch->bearing[0] * o.scale;
      float ypos = o.y - (ch->size[1] - ch->bearing[1]) * o.scale;
      float w = ch->size[0] * o.scale;
      float h = ch->size[1] * o.scale;

      MatrixX4f vertices(6, 4);
      vertices << xpos, ypos + h,     0.0f, 0.0f,
                  xpos, ypos,         0.0f, 1.0f,
                  xpos + w, ypos,     1.0f, 1.0f,
                  xpos, ypos + h,     0.0f, 0.0f,
                  xpos + w, ypos,     1.0f, 1.0f,
                  xpos + w, ypos + h, 1.0f, 0.0f;
      ch->texture.bind();
      buf_.replace(vertices);      

      glDrawArrays(GL_TRIANGLES, 0, 6);
      
      // Advance is stored in number of 1/64 pixels
      x += (ch->advance >> 6) * o.scale;
    }
  }
}

// Free functions
void cannon::graphics::init_glfw() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  glfwWindowHint(GLFW_SAMPLES, 16);
}

void cannon::graphics::init_glad() {
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    throw std::runtime_error("Could not initialize glad");
  }
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
