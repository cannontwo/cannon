#ifndef CANNON_GRAPHICS_WINDOW_H
#define CANNON_GRAPHICS_WINDOW_H 

#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace cannon {
  namespace graphics {

    // Free functions
    void init_glfw();
    void init_glad();

    // Callbacks
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    class Window {
      public:
        Window(unsigned width = 800, unsigned height = 600, const std::string& name = "Test") {
          window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL); 

          if (window == NULL) {
            glfwTerminate();
            throw std::runtime_error("Could not create GLFW window");
          }

          make_current();
          init_glad();
          set_viewport(0, 0, width, height);

          register_callbacks();
        }

        ~Window() {
          glfwTerminate();
        }

        void make_current();
        void set_viewport(unsigned x, unsigned y, unsigned width, unsigned height);
        void register_callbacks();

        template <typename F>
        void render_loop(F f) {
          while (!glfwWindowShouldClose(window)) {
            glfwSwapBuffers(window);
            glfwPollEvents();
            f();
          }
        }

        GLFWwindow *window;
    };

    Window create_window();

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_WINDOW_H */
