#pragma once
#ifndef CANNON_GRAPHICS_WINDOW_H
#define CANNON_GRAPHICS_WINDOW_H 

#include <vector>
#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Eigen/Dense>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <cannon/graphics/font.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/projection.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/utils/thread_pool.hpp>
#include <cannon/utils/class_forward.hpp>

using namespace Eigen;

using namespace cannon::utils;

namespace cannon {
  namespace graphics {

    CANNON_CLASS_FORWARD(VertexArrayObject);
    CANNON_CLASS_FORWARD(Framebuffer);

    struct OverlayText {
      float x;
      float y;
      float scale;
      std::string text;
      std::function<void(OverlayText&)> update;
    };

    struct CircularBuffer {
      void add_point(float p) {
        data[offset] = p;
        offset = (offset + 1) % IM_ARRAYSIZE(data);
      }

      float data[90] = {};
      int offset = 0;
    };

    struct ImageData {
      std::string path;
      std::shared_ptr<std::vector<char>> buffer;
      int num_channels;
      int width;
      int height;
      int stride;
    };

    // Free functions
    void init_glfw();
    void init_glad();

    void save_image_func(std::shared_ptr<ImageData> data);

    // Callbacks
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum
        severity, GLsizei length, const GLchar *message, 
        const void *userParam);
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    class Window {
      public:

        Window(int w = 800, int h = 600, const std::string& name = "Test");

        ~Window();

        void make_current();
        void set_viewport(unsigned x, unsigned y, unsigned width, unsigned height);

        GLFWwindow* get_gl_window();
        
        void set_clear_color(Vector4f color);
        void set_text_color(Vector4f color);
        void set_wireframe_mode();
        void set_fill_mode();

        void display_text(float x, float y, float scale, 
            const std::string& text, std::function<void(OverlayText&)> update);
        void display_fps(float x, float y, float scale);

        void enable_depth_test();
        void disable_depth_test();

        void enable_stencil_test();
        void disable_stencil_test();
        
        void enable_face_culling();
        void disable_face_culling();

        void render_to_framebuffer(FramebufferPtr fb);
        void draw_from_framebuffer(FramebufferPtr fb);
        void save_image(const std::string &path);
        ImageData get_image(const std::string &path = "") const;

        void close() const;

        void render_loop(std::function<void()> f, bool clear = true) {
          while (!glfwWindowShouldClose(window)) {
            if (clear) {
              glClearColor(clear_color_[0], clear_color_[1], clear_color_[2], clear_color_[3]);
              glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            }

            glQueryCounter(gl_time_query_, GL_TIMESTAMP);

            GLint64 start_time;
            glGetInteger64v(GL_TIMESTAMP, &start_time);

            float cur_time = glfwGetTime();
            delta_time_ = cur_time - last_frame_time_;

            draw(f);

            if (recording_) {
              save_image(std::string("img/") +
                  std::to_string(recording_start_time_) + std::string("/") +
                  std::to_string(recording_frame_) + ".png");
              recording_frame_ += 1;
            }

            elapsed_ = glfwGetTime() - last_frame_time_;
            last_frame_time_ = glfwGetTime();
            float smoothing = 0.9;
            float estimate = 1.0 / (elapsed_);

            fps_ = (fps_ * smoothing) + (estimate * (1.0 - smoothing));
            time_cbuf_.add_point(elapsed_);
            fps_cbuf_.add_point(fps_);

            glfwPollEvents();

            int done = 0;
            while (!done) {
              glGetQueryObjectiv(gl_time_query_, GL_QUERY_RESULT_AVAILABLE, &done);
            }

            GLuint64 end_time;
            glGetQueryObjectui64v(gl_time_query_, GL_QUERY_RESULT, &end_time);
            draw_time_ = (double)(end_time - start_time) / 1e9f;
            draw_time_cbuf_.add_point(draw_time_);
          }
        }

        void draw(std::function<void()> f);

        int width;
        int height;

        friend class Viewer3D;
        friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

      private:
        void register_callbacks();
        void init_text_shader();
        void draw_overlays();
        void write_imgui();

        GLFWwindow *window;

        Font font_;
        Vector4f text_color_;
        ShaderProgram text_program_;
        Vector4f clear_color_;
        VertexArrayObjectPtr vao_;
        VertexBuffer buf_;
        std::vector<OverlayText> overlays_;

        bool render_to_framebuffer_ = false;
        FramebufferPtr render_fb_;
        bool draw_from_framebuffer_ = false;
        FramebufferPtr draw_fb_;

        float delta_time_ = 0.0;
        float last_frame_time_ = 0.0;
        float fps_ = 0.0;
        double elapsed_ = 0.0;

        bool recording_ = false;
        double recording_start_time_;
        int recording_frame_;

        CircularBuffer time_cbuf_;
        CircularBuffer fps_cbuf_;
        CircularBuffer draw_time_cbuf_;

        ThreadPool<ImageData> save_pool_;

        unsigned int gl_time_query_;
        double draw_time_;
    };

    // This is necessary to prevent segfault on destruction; is there a better
    // way to encapsulate this?
    void terminate_opengl_context(GLFWwindow *w);

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_WINDOW_H */
