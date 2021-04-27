#pragma once
#ifndef CANNON_GRAPHICS_RENDER_PASS_H
#define CANNON_GRAPHICS_RENDER_PASS_H 

/*!
 * \file cannon/graphics/render_pass.hpp
 * File containing RenderPass class definition.
 */

#include <memory>
#include <functional>

#include <imgui.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cannon/graphics/window.hpp>
#include <cannon/graphics/framebuffer.hpp>
#include <cannon/graphics/shader_program.hpp>

namespace cannon {
  namespace graphics {

    /*!
     * \brief Class representing a render pass in the deferred rendering pipeline.
     *
     * This class represents a stage in the deferred rendering pipeline,
     * encapsulating a framebuffer, shaders to apply, and function that does all of the
     * necessary rendering computation. This class is designed such that later
     * rendering passes can access the results of this rendering pass to do
     * further computations. 
     */
    class RenderPass {
      public:
        RenderPass() = delete;

        /*!
         * Constructor taking a name, framebuffer, shader, and processing function.
         *
         * Does not affect OpenGL state.
         */
        RenderPass(const std::string &name, std::shared_ptr<Framebuffer> fb,
            std::shared_ptr<ShaderProgram> p, std::function<void()> f) :
          name(name), framebuffer(fb), draw_func(f) {

            programs.push_back(p);

            glGenQueries(1, &gl_front_query_);
            glGenQueries(1, &gl_back_query_);

            // Dummy to prevent error on first frame
            glBeginQuery(GL_TIME_ELAPSED, gl_front_query_);
            glEndQuery(GL_TIME_ELAPSED);
          }

        /*!
         * Constructor taking a name, framebuffer, vector of shaders, and processing function.
         *
         * Does not affect OpenGL state.
         */
        RenderPass(const std::string &name, std::shared_ptr<Framebuffer> fb,
            std::vector<std::shared_ptr<ShaderProgram>> programs, std::function<void()> f) :
          name(name), framebuffer(fb), programs(programs), draw_func(f) {
            glGenQueries(1, &gl_front_query_);
            glGenQueries(1, &gl_back_query_);

            // Dummy to prevent error on first frame
            glBeginQuery(GL_TIME_ELAPSED, gl_front_query_);
            glEndQuery(GL_TIME_ELAPSED);
          }

        /*!
         * Copy constructor.
         */
        RenderPass(RenderPass& o) : name(o.name), framebuffer(o.framebuffer),
        programs(o.programs), draw_func(o.draw_func),
        gl_front_query_(o.gl_front_query_), gl_back_query_(o.gl_back_query_) {}

        /*!
         * Move constructor.
         */
        RenderPass(RenderPass&& o) : name(o.name), framebuffer(o.framebuffer),
        programs(o.programs),
        draw_func(o.draw_func),gl_front_query_(o.gl_front_query_),
        gl_back_query_(o.gl_back_query_) {
          o.gl_front_query_ = 0;
          o.gl_back_query_ = 0; 
        }

        /*!
         * Destructor. Frees OpenGL state.
         */
        ~RenderPass() {
          if (glfwGetCurrentContext() != NULL) {
            glDeleteQueries(1, &gl_front_query_);
            glDeleteQueries(1, &gl_back_query_);
          }
        }

        /*!
         * Method to perform the render pass represented by this object by
         * binding this object's framebuffer and running this object's
         * processing function.
         *
         * Does not affect OpenGL state.
         */
        void run();
        
        /*! 
         * Write ImGui controls corresponding to parameters of this render pass.
         *
         * Does not affect OpenGL state.
         */
        void write_imgui();

        /*!
         * Utility method to record statistics for display with ImGUI.
         *
         * \param t The time taken in this render pass.
         */
        // Does not affect OpenGL state
        void set_time_taken(double t);

        std::string name; //!< The name of this render pass

        std::shared_ptr<Framebuffer> framebuffer; //!< The framebuffer for this render pass 
        std::vector<std::shared_ptr<ShaderProgram>> programs; //!< Shaders for this render pass
        std::function<void()> draw_func; //!< Rendering function for this render pass

      private:

        /*!
         * Utility method to swap OpenGL time queries and allow for collecting
         * execution time statistics.
         */
        void swap_queries_();

        double time_taken_; //!< The time taken in this render pass.

        unsigned int gl_front_query_; //!< OpenGL time query currently used for reading
        unsigned int gl_back_query_; //!< OpenGL time query currently used for recording

        CircularBuffer time_cbuf_; //!< Buffer used by ImGui to display time statistics
    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_RENDER_PASS_H */
