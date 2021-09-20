#include <cannon/graphics/window.hpp>

#include <filesystem>
#include <thread>
#include <stdexcept>

#include <stb_image/stb_image_write.h>

#include <cannon/graphics/character.hpp>
#include <cannon/graphics/texture.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/framebuffer.hpp>
#include <cannon/graphics/opengl_state.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/font.hpp>
#include <cannon/graphics/projection.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/text_renderer.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::graphics;
using namespace cannon::log;

Window::Window(int w, int h, const std::string &name)
    : width(w), height(h), text_renderer_(nullptr),
      save_pool_(save_image_func) {

  init_glfw();
  window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL); 

  if (window == NULL) {
    glfwTerminate();
    throw std::runtime_error("Could not create GLFW window");
  }

  make_current();
  init_glad();

  glEnable(GL_MULTISAMPLE);
  // V-Sync
  glfwSwapInterval(1);
  set_viewport(0, 0, width, height);

  register_callbacks();
  set_clear_color(Vector4f(0.02f, 0.07f, 0.09f, 1.0f));

  // Setting up overlay text rendering
  auto font = std::make_shared<Font>(false);
  font->init();
  text_renderer_ = std::make_shared<TextRenderer>(width, height, font);
  set_text_color(Vector4f(1.0, 0.0, 0.0, 1.0));

  // Setting up ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  //ImPlot::CreateContext();
  ImGui::GetIO();

  ImGui::StyleColorsDark();
  ImGui::SetColorEditOptions(ImGuiColorEditFlags_Float |
      ImGuiColorEditFlags_DisplayHSV |
      ImGuiColorEditFlags_PickerHueWheel);
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  const char *glsl_version = "#version 330 core";
  ImGui_ImplOpenGL3_Init(glsl_version);

  glGenQueries(1, &gl_time_query_);
}

Window::~Window() {
  log_info("Destructing window");

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  //ImPlot::DestroyContext();
  ImGui::DestroyContext();

  //glfwMakeContextCurrent(NULL);
}

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

  if (text_renderer_ != nullptr) {
    text_renderer_->width() = static_cast<unsigned int>(width);
    text_renderer_->height() = static_cast<unsigned int>(height);
  }
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
  text_renderer_->text_color() = color;
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
  overlays_.back().update(overlays_.back());

  auto box = text_renderer_->compute_bounding_box(text, x, y, scale);
  log_info("Overlay will be draw at (", box.x, ",", box.y, ") with width",
           box.width, "and height", box.height);
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
  for (auto& o : overlays_) {
    o.update(o);
    text_renderer_->draw(o.text, o.x, o.y, o.scale);
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

void cannon::graphics::debug_message_callback(GLenum /*source*/, GLenum /*type*/, GLuint /*id*/, GLenum
    severity, GLsizei /*length*/, const GLchar *message, 
    const void * /*userParam*/) {
  if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) 
    log_warning(message);
}

void cannon::graphics::key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// Free Functions
void cannon::graphics::terminate_opengl_context(GLFWwindow *w) {
  glfwDestroyWindow(w);
  glfwTerminate();
}
