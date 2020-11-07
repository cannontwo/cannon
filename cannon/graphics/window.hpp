#ifndef CANNON_GRAPHICS_WINDOW_H
#define CANNON_GRAPHICS_WINDOW_H 

#include <vector>
#include <stdexcept>
#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image_write.h>
#include <Eigen/Dense>

#include <cannon/log/registry.hpp>
#include <cannon/graphics/font.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>
#include <cannon/graphics/projection.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/vertex_array_object.hpp>

// TODO
//#include <cannon/graphics/input_handlers.hpp>

using namespace Eigen;

namespace cannon {
  namespace graphics {

    struct OverlayText {
      float x;
      float y;
      float scale;
      std::string text;
      std::function<void(OverlayText&)> update;
    };

    // Free functions
    void init_glfw();
    void init_glad();

    // Callbacks
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum
        severity, GLsizei length, const GLchar *message, 
        const void *userParam);
    void process_input(GLFWwindow* window);

    class Window {
      public:
        Window(int w = 800, int h = 600, const std::string& name = "Test"):
          width(w), height(h), font_(false), text_program_(false), vao_(nullptr), buf_() {
          init_glfw();
          window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL); 

          if (window == NULL) {
            glfwTerminate();
            throw std::runtime_error("Could not create GLFW window");
          }

          make_current();
          init_glad();

          glEnable(GL_MULTISAMPLE);
          set_viewport(0, 0, width, height);

          register_callbacks();
          set_clear_color(Vector4f(0.02f, 0.07f, 0.09f, 1.0f));
          set_text_color(Vector4f(1.0, 0.0, 0.0, 1.0));
          font_.init();
          text_program_.init();
          vao_ = std::make_shared<VertexArrayObject>();
          buf_.init(vao_);
          init_text_shader();
        }

        ~Window() {
          glfwTerminate();
        }

        void make_current();
        void set_viewport(unsigned x, unsigned y, unsigned width, unsigned height);
        
        void set_clear_color(Vector4f color);
        void set_text_color(Vector4f color);
        void set_wireframe_mode();
        void set_fill_mode();

        void display_text(float x, float y, float scale, 
            const std::string& text, std::function<void(OverlayText&)> update);
        void display_fps(float x, float y, float scale);

        void enable_depth_test();
        void disable_depth_test();

        void save_image(const std::string &path);

        template <typename F>
        void render_loop(F f) {
          while (!glfwWindowShouldClose(window)) {
            float cur_time = glfwGetTime();
            delta_time_ = cur_time - last_frame_time_;
            last_frame_time_ = cur_time;

            process_input(window);

            glClearColor(clear_color_[0], clear_color_[1], clear_color_[2], clear_color_[3]);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glfwGetFramebufferSize(window, &width, &height);

            f();

            draw_overlays();

            glfwSwapBuffers(window);
            glfwPollEvents();
          }
        }

        template <typename F>
        void render_once(F f) {
          if (!glfwWindowShouldClose(window)) {
            // Call rendering process three times because OpenGL is double-buffered by default
            for (int i = 0; i < 3; i++) {
              float cur_time = glfwGetTime();
              delta_time_ = cur_time - last_frame_time_;
              last_frame_time_ = cur_time;

              process_input(window);

              glClearColor(clear_color_[0], clear_color_[1], clear_color_[2], clear_color_[3]);
              glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

              glfwGetFramebufferSize(window, &width, &height);

              f();

              draw_overlays();

              glfwSwapBuffers(window);
              glfwPollEvents();
            }
          }
        }

        int width;
        int height;

        friend class Viewer3D;

      private:
        void register_callbacks();
        void init_text_shader();
        void draw_overlays();

        GLFWwindow *window;

        Font font_;
        Vector4f text_color_;
        ShaderProgram text_program_;
        Vector4f clear_color_;
        std::shared_ptr<VertexArrayObject> vao_;
        VertexBuffer buf_;
        std::vector<OverlayText> overlays_;

        float delta_time_ = 0.0;
        float last_frame_time_ = 0.0;
    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_WINDOW_H */
