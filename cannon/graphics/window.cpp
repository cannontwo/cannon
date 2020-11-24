#include <cannon/graphics/window.hpp>

using namespace cannon::graphics;
using namespace cannon::log;

void Window::close() const {
  glfwSetWindowShouldClose(window, true);
}

void Window::draw(std::function<void()> f) {
  if (render_to_framebuffer_)
    render_fb_->bind();

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  f();

  draw_overlays();

  write_imgui();

  if (draw_from_framebuffer_) {
    draw_fb_->display();
    draw_fb_->unbind();
  }

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwSwapBuffers(window);
}

void Window::make_current() {
  glfwMakeContextCurrent(window);
}

void Window::set_viewport(unsigned x, unsigned y, unsigned width, unsigned height) {
  glViewport(x, y, width, height);
}

void Window::register_callbacks() {
  glfwSetWindowUserPointer(window, this);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);
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

void Window::display_text(float x, float y, float scale, const std::string
    &text, std::function<void(OverlayText&)> update) {
  overlays_.push_back({x, y, scale, text, update});
}

void Window::display_fps(float x, float y, float scale) {
  float last_time = glfwGetTime();
  float fps = 0.0;

  overlays_.push_back({x, y, scale, "", [last_time,fps](OverlayText& o) mutable {
      float smoothing = 0.9;
      float estimate = 1.0 / (glfwGetTime() - last_time);
      fps = (fps * smoothing) + (estimate * (1.0 - smoothing));
      
      std::stringstream ss;
      ss.precision(2);
      ss << fps;
      o.text.replace(o.text.begin(), o.text.end(), ss.str());

      last_time = glfwGetTime();
      }});
}

void Window::init_text_shader() {
  VertexShader v = load_vertex_shader("shaders/char_shader.vert");
  FragmentShader f = load_fragment_shader("shaders/char_shader.frag");

  text_program_.attach_shader(v);
  text_program_.attach_shader(f);
  text_program_.link();

  // Reserve space for display quads
  MatrixX4f vertices(MatrixX4f::Zero(6, 4));
  buf_.bind();
  buf_.buffer(vertices);
}

void Window::write_imgui() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("Recording")) {
      if (ImGui::Button("Take Screenshot")) {
        std::filesystem::create_directory("img");
        save_image(std::string("img/screenshot_") +
            std::to_string(glfwGetTime()) + ".png");
      }

      if (!recording_) {
        if (ImGui::Button("Record Video")) {
          recording_start_time_ = glfwGetTime();
          recording_ = true;
          recording_frame_ = 0;

          std::filesystem::create_directory(std::string("img/") +
              std::to_string(recording_start_time_));
        }
      } else {
        if (ImGui::Button("Stop Recording")) {
          recording_ = false;
        }
      }

      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Statistics")) {
      ImGui::Text("CPU time: %f", elapsed_);
      ImGui::PlotLines("Times", time_cbuf_.data, IM_ARRAYSIZE(time_cbuf_.data), time_cbuf_.offset, NULL, 0.0, 0.05);
      ImGui::Text("GPU time: %f", draw_time_);
      ImGui::PlotLines("Times", draw_time_cbuf_.data, IM_ARRAYSIZE(draw_time_cbuf_.data), draw_time_cbuf_.offset, NULL, 0.0, 0.05);
      ImGui::Text("FPS: %f", fps_);
      ImGui::PlotLines("FPS", fps_cbuf_.data, IM_ARRAYSIZE(fps_cbuf_.data), fps_cbuf_.offset, NULL, 0.0, 100.0);
      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }
}

void Window::draw_overlays() {
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  vao_->bind();
  text_program_.set_uniform("projection", make_orthographic(0.0f, (float)width,
        0.0f, (float)height, -1.0, 1.0));
  text_program_.set_uniform("textColor", text_color_);
  text_program_.set_uniform("text", 0);
  text_program_.activate();

  for (auto& o : overlays_) {
    o.update(o);

    // Using negative overlay position to mean relative to top-right
    float x = o.x;
    if (x < 0.0)
      x = width + x;

    float y = o.y;
    if (y < 0.0)
      y = height + y;

    std::string::const_iterator c;   

    for (c = o.text.begin(); c != o.text.end(); c++) {
      std::shared_ptr<Character> ch = font_.get_char(*c);

      float xpos = x + ch->bearing[0] * o.scale;
      float ypos = y - (ch->size[1] - ch->bearing[1]) * o.scale;
      float w = ch->size[0] * o.scale;
      float h = ch->size[1] * o.scale;

      MatrixX4f vertices(6, 4);
      vertices << xpos, ypos + h,     0.0f, 0.0f,
                  xpos, ypos,         0.0f, 1.0f,
                  xpos + w, ypos,     1.0f, 1.0f,
                  xpos, ypos + h,     0.0f, 0.0f,
                  xpos + w, ypos,     1.0f, 1.0f,
                  xpos + w, ypos + h, 1.0f, 0.0f;
      ch->texture->bind();
      buf_.replace(vertices);      
      buf_.bind();
      OpenGLState s;
      log_info(s);

      glDrawArrays(GL_TRIANGLES, 0, 6);
      
      // Advance is stored in number of 1/64 pixels
      x += (ch->advance >> 6) * o.scale;
    }
  }

  glDisable(GL_BLEND);
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
}

void cannon::graphics::save_image_func(std::shared_ptr<ImageData> data) {
  stbi_flip_vertically_on_write(true);
  if (stbi_write_png(data->path.c_str(), data->width, data->height,
        data->num_channels, data->buffer->data(), data->stride) == 0) {
    throw std::runtime_error("Could not write OpenGL render to file"); 
  }
}

void Window::enable_depth_test() {
  glEnable(GL_DEPTH_TEST);
}

void Window::disable_depth_test() {
  glDisable(GL_DEPTH_TEST);
}

void Window::enable_stencil_test() {
  glEnable(GL_STENCIL_TEST);
}

void Window::disable_stencil_test() {
  glDisable(GL_STENCIL_TEST);
}

void Window::enable_face_culling() {
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
}

void Window::disable_face_culling() {
  glDisable(GL_CULL_FACE);
}

void Window::save_image(const std::string &path) {
  ImageData data = get_image(path);
  save_pool_.enqueue(data);
}

ImageData Window::get_image(const std::string &path) const {
  int num_channels = 3;
  int stride = num_channels * width;
  stride += (stride % 4) ? (4 - stride % 4) : 0; // Alignment

  int buffer_size = stride * height;
  std::shared_ptr<std::vector<char>> buffer = std::make_shared<std::vector<char>>(buffer_size);

  if (render_to_framebuffer_)
    render_fb_->unbind();

  // Configure reading
  glPixelStorei(GL_PACK_ALIGNMENT, 4);
  glReadBuffer(GL_FRONT);

  // Read into buffer
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer->data());

  if (render_to_framebuffer_)
    render_fb_->bind();

  int tmp_width = width;
  int tmp_height = height;

  ImageData data = {std::string(path), buffer, num_channels, tmp_width, tmp_height, stride};
  return data;
}

void Window::render_to_framebuffer(std::shared_ptr<Framebuffer> fb) {
  render_to_framebuffer_ = true;
  render_fb_ = fb;
}

void Window::draw_from_framebuffer(std::shared_ptr<Framebuffer> fb) {
  draw_from_framebuffer_ = true;
  draw_fb_ = fb;
}

GLFWwindow* Window::get_gl_window() {
  return window;
}

// Callbacks
void cannon::graphics::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);

  Window *w = (Window*)glfwGetWindowUserPointer(window);
  w->width = width;
  w->height = height;
  w->set_viewport(0, 0, width, height);

  log_info("Resize drawing");

  // For smooth resizing
  w->draw([](){});
}

void cannon::graphics::debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum
    severity, GLsizei length, const GLchar *message, 
    const void *userParam) {
  if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) 
    log_warning(message);
}

void cannon::graphics::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// Free Functions
void cannon::graphics::terminate_opengl_context(GLFWwindow *w) {
  glfwDestroyWindow(w);
  glfwTerminate();
}
