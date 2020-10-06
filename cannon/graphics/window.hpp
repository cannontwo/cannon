#ifndef CANNON_GRAPHICS_WINDOW_H
#define CANNON_GRAPHICS_WINDOW_H 

#include <vector>
#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image_write.h>
#include <Eigen/Dense>

#include <cannon/log/registry.hpp>

// TODO
//#include <cannon/graphics/input_handlers.hpp>

using namespace Eigen;

namespace cannon {
  namespace graphics {

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
        Window(int w = 800, int h = 600, const std::string& name = "Test"): width(w), height(h) {
          init_glfw();
          window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL); 

          if (window == NULL) {
            glfwTerminate();
            throw std::runtime_error("Could not create GLFW window");
          }

          make_current();
          init_glad();
          set_viewport(0, 0, width, height);

          register_callbacks();
          set_clear_color(Vector4f(0.2f, 0.3f, 0.3f, 1.0f));
        }

        ~Window() {
          glfwTerminate();
        }

        void make_current();
        void set_viewport(unsigned x, unsigned y, unsigned width, unsigned height);
        void register_callbacks();
        
        void set_clear_color(Vector4f color);
        void set_wireframe_mode();
        void set_fill_mode();

        void enable_depth_test();
        void disable_depth_test();

        void save_image(const std::string &path);

        template <typename F>
        void render_loop(F f) {
          while (!glfwWindowShouldClose(window)) {
            process_input(window);

            glClearColor(clear_color_[0], clear_color_[1], clear_color_[2], clear_color_[3]);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glfwGetFramebufferSize(window, &width, &height);

            f();

            glfwSwapBuffers(window);
            glfwPollEvents();
          }
        }

        int width;
        int height;

      private:
        GLFWwindow *window;
        Vector4f clear_color_;
    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_WINDOW_H */
