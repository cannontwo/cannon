#pragma once
#ifndef CANNON_GRAPHICS_WINDOW_H
#define CANNON_GRAPHICS_WINDOW_H 

/*!
 * \file cannon/graphics/window.hpp
 * \brief File containing Window class definition.
 */

#include <vector>
#include <functional>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Eigen/Dense>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <cannon/utils/thread_pool.hpp>
#include <cannon/utils/class_forward.hpp>

using namespace Eigen;

using namespace cannon::utils;

namespace cannon {
  namespace graphics {

    CANNON_CLASS_FORWARD(VertexArrayObject);
    CANNON_CLASS_FORWARD(Framebuffer);
    CANNON_CLASS_FORWARD(Font);
    CANNON_CLASS_FORWARD(ShaderProgram);
    CANNON_CLASS_FORWARD(VertexBuffer);
    CANNON_CLASS_FORWARD(TextRenderer);

    /*!
     * \brief Utility struct for overlay text rendered at the top level of the
     * window.
     */
    struct OverlayText {
      float x; //!< Horizontal coordinate for text 
      float y; //!< Vertical coordinate for text
      float scale; //!< Text scale
      std::string text; //!< String to render
      std::function<void(OverlayText&)> update; //!< Text update function to run each frame
    };

    /*!
     * \brief Utility struct for easily displaying line plots that are updated
     * over time in ImGUI. Has the semantics of a circular buffer.
     */
    struct CircularBuffer {
      
      /*!
       * \brief Add a point to this buffer to be rendered.
       *
       * \param p Point to add.
       */
      void add_point(float p) {
        data[offset] = p;
        offset = (offset + 1) % IM_ARRAYSIZE(data);
      }

      float data[90] = {}; //!< Data points held by this buffer.
      int offset = 0; //!< Current offset into the buffer for insertion.
    };

    /*!
     * \brief Struct representing stored image data.
     */
    struct ImageData {
      std::string path; //!< Optional path to store image data
      std::shared_ptr<std::vector<char>> buffer; //!< Actually image data
      int num_channels; //!< Number of channels in stored image data
      int width; //!< Width of image
      int height; //!< Height of image
      int stride; //!< Pixel stride 
    };

    // Free functions
    
    /*!
     * \brief Initialize GLFW resources
     */
    void init_glfw();

    /*!
     * \brief Initialize GLAD OpenGL mappings
     */
    void init_glad();

    /*!
     * \brief Callback function to write stored image data to a file.
     *
     * \param data The image data to save.
     */
    void save_image_func(std::shared_ptr<ImageData> data);

    // Callbacks
    
    /*!
     * \brief Resize callback for Window. 
     *
     * \param window OpenGL context
     * \param width New width of the window
     * \param height New height of the window
     */
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    /*!
     * \brief Debug message callback for Window.
     *
     * \param source Source of debug message in OpenGL
     * \param type Type of debug message
     * \param id Message ID
     * \param severity Message severity
     * \param length Message length
     * \param message Message contents
     * \param userParam User-defined parameter
     */
    void debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum
        severity, GLsizei length, const GLchar *message, 
        const void *userParam);

    /*!
     * \brief Key callback for Window.
     *
     * \param window OpenGL context
     * \param key Key pressed
     * \param scancode Operating system scancode
     * \param action Kind of key press
     * \param mods Modifying keys held down
     */
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    /*!
     * \brief Class representing an OpenGL window and encapsulating much of the
     * behavior of an OpenGL context.
     */
    class Window {
      public:

        /*!
         * \brief Constructor taking an optional width, height, and window name.
         *
         * \param w Width of window to construct
         * \param h Height of window to construct
         * \param name Name of window to construct
         */
        Window(int w = 800, int h = 600, const std::string& name = "Test");

        /*!
         * \brief Destructor. Destroys OpenGL context.
         */
        ~Window();

        /*!
         * \brief Make this window's OpenGL context current.
         */
        void make_current();
        
        /*!
         * \brief Set OpenGL rendering viewport.
         *
         * \param x New upper-left corner position x-coordinate
         * \param y New upper-left corner position y-coordinate
         * \param width New viewport width
         * \param height new viewport height
         */
        void set_viewport(unsigned x, unsigned y, unsigned width, unsigned height);

        /*!
         * \brief Get the OpenGL context held by this window.
         *
         * \returns The context
         */
        GLFWwindow* get_gl_window();
        
        /*!
         * \brief Set per-frame clear color.
         *
         * \param color The new clear color.
         */
        void set_clear_color(Vector4f color);

        /*!
         * \brief Set overlay text color.
         *
         * \param color The new overlay text color.
         */
        void set_text_color(Vector4f color);

        /*!
         * \brief Set rendering mode for this window to wireframe.
         */ 
        void set_wireframe_mode();

        /*!
         * \brief Set rendering mode for this window to fill.
         */
        void set_fill_mode();

        /*!
         * \brief Add overlay text to this window.
         *
         * \param x Position for text x-coordinate
         * \param y Position for text y-coordinate
         * \param scale Text scale
         * \param text Initial text to render
         * \param update Per-frame update function
         */
        void display_text(float x, float y, float scale, 
            const std::string& text, std::function<void(OverlayText&)> update);

        /*!
         * \brief Display current fps as overlay text.
         *
         * \param x Position for fps display x-coordinate
         * \param y Position for fps display y-coordinate
         * \param scale Text scale for fps display
         */
        void display_fps(float x, float y, float scale);

        /*!
         * \brief Enable depth testing for this window.
         */
        void enable_depth_test();
        
        /*!
         * \brief Disable depth testing for this window.
         */
        void disable_depth_test();

        /*!
         * \brief Enable stencil testing for this window.
         */
        void enable_stencil_test();

        /*!
         * \brief Disable stencil testing for this window.
         */
        void disable_stencil_test();
        
        /*!
         * \brief Enable face culling for this window.
         */
        void enable_face_culling();
        
        /*!
         * \brief Disable face culling for this window.
         */
        void disable_face_culling();

        /*!
         * \brief Set this window to render to the input framebuffer.
         *
         * \param fb The framebuffer to render to.
         */
        void render_to_framebuffer(FramebufferPtr fb);
        
        /*!
         * \brief Set this window to read from the input framebuffer for
         * rendering.
         *
         * \param fb The framebuffer to draw from.
         */
        void draw_from_framebuffer(FramebufferPtr fb);

        /*!
         * \brief Save currently rendered image to the input file path.
         *
         * \param path Path to save current image to.
         */
        void save_image(const std::string &path);

        /*!
         * \brief Get image data for the currently rendered image.
         *
         * \param path Path for saving to associate with returned image data.
         * \returns Image data for the rendered image.
         */
        ImageData get_image(const std::string &path = "") const;

        /*!
         * \brief Close this window.
         */
        void close() const;

        /*!
         * \brief Blocking render loop function. Takes a user-specified update
         * function to run on each frame.
         *
         * \param f User update function.
         * \param clear Whether to clear the display after each frame.
         */
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

        /*!
         * \brief Core draw step, executed on each frame.
         *
         * \param f User update function to run on each frame.
         */
        void draw(std::function<void()> f);

        int width; //!< Current width of this window
        int height; //!< Current height of this window

        friend class Viewer3D;
        friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

      private:
        
        /*!
         * \brief Register all GLFW callbacks for this window.
         */
        void register_callbacks();
        
        /*!
         * \brief Draw overlay text for this window.
         */
        void draw_overlays();
        
        /*!
         * \brief Write ImGUI controls for this window.
         */
        void write_imgui();

        GLFWwindow *window; //!< OpenGL window object

        TextRendererPtr text_renderer_; //!< Renderer for overlay text
        Vector4f clear_color_; //!< Display clear color
        std::vector<OverlayText> overlays_; //!< Overlay text instances

        bool render_to_framebuffer_ = false; //!< Whether to render to framebuffer
        FramebufferPtr render_fb_; //!< Framebuffer to render to
        bool draw_from_framebuffer_ = false; //!< Whether to draw from a framebuffer
        FramebufferPtr draw_fb_; //!< Framebuffer to draw from

        float delta_time_ = 0.0; //!< Time since last frame was rendered
        float last_frame_time_ = 0.0; //!< Render time of previous frame
        float fps_ = 0.0; //!, Frames per second estimate
        double elapsed_ = 0.0; //!< Time taken to render on CPU

        bool recording_ = false; //!< Whether frames are being recorded
        double recording_start_time_; //!< Start time of recording
        int recording_frame_; //!< Index of current frame in recording

        CircularBuffer time_cbuf_; //!< Circular buffer displaying render times
        CircularBuffer fps_cbuf_; //!< Circular buffer displaying fps
        CircularBuffer draw_time_cbuf_; //!< Circular buffer displaying draw time

        ThreadPool<ImageData> save_pool_; //!< Thread pool for saving rendered frames

        unsigned int gl_time_query_; //!< OpenGL time query object
        double draw_time_; //!< Time taken to render on GPU
    };

    /*!
     * \brief Function to destroy input OpenGL context.
     *
     * This is necessary to prevent segfault on destruction; is there a better
     * way to encapsulate this?
     *
     * \param w The OpenGL context to destroy.
     */
    void terminate_opengl_context(GLFWwindow *w);

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_WINDOW_H */
