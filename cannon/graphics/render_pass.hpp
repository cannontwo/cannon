#ifndef CANNON_GRAPHICS_RENDER_PASS_H
#define CANNON_GRAPHICS_RENDER_PASS_H 

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

    class RenderPass {
      public:
        RenderPass() = delete;

        // Does not affect OpenGL state
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

        // Does not affect OpenGL state
        RenderPass(const std::string &name, std::shared_ptr<Framebuffer> fb,
            std::vector<std::shared_ptr<ShaderProgram>> programs, std::function<void()> f) :
          name(name), framebuffer(fb), programs(programs), draw_func(f) {
            glGenQueries(1, &gl_front_query_);
            glGenQueries(1, &gl_back_query_);

            // Dummy to prevent error on first frame
            glBeginQuery(GL_TIME_ELAPSED, gl_front_query_);
            glEndQuery(GL_TIME_ELAPSED);
          }

        RenderPass(RenderPass& o) : name(o.name), framebuffer(o.framebuffer),
        programs(o.programs), draw_func(o.draw_func),
        gl_front_query_(o.gl_front_query_), gl_back_query_(o.gl_back_query_) {}

        RenderPass(RenderPass&& o) : name(o.name), framebuffer(o.framebuffer),
        programs(o.programs),
        draw_func(o.draw_func),gl_front_query_(o.gl_front_query_),
        gl_back_query_(o.gl_back_query_) {
          o.gl_front_query_ = 0;
          o.gl_back_query_ = 0; 
        }

        ~RenderPass() {
          if (glfwGetCurrentContext() != NULL) {
            glDeleteQueries(1, &gl_front_query_);
            glDeleteQueries(1, &gl_back_query_);
          }
        }

        // Does not affect OpenGL state
        void run();
        
        // Does not affect OpenGL state
        void write_imgui();

        // Does not affect OpenGL state
        void set_time_taken(double t);

        std::string name;

        std::shared_ptr<Framebuffer> framebuffer;
        std::vector<std::shared_ptr<ShaderProgram>> programs;
        std::function<void()> draw_func;

      private:
        void swap_queries_();

        double time_taken_;

        unsigned int gl_front_query_;
        unsigned int gl_back_query_;

        CircularBuffer time_cbuf_;
    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_RENDER_PASS_H */
