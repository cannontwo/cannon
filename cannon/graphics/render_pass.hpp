#ifndef CANNON_GRAPHICS_RENDER_PASS_H
#define CANNON_GRAPHICS_RENDER_PASS_H 

#include <memory>
#include <functional>

#include <imgui.h>

#include <cannon/graphics/framebuffer.hpp>
#include <cannon/graphics/shader_program.hpp>

namespace cannon {
  namespace graphics {

    class RenderPass {
      public:
        RenderPass() = delete;

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
          glDeleteQueries(1, &gl_front_query_);
          glDeleteQueries(1, &gl_back_query_);
        }

        void run() {
          glBeginQuery(GL_TIME_ELAPSED, gl_back_query_);

          framebuffer->bind();
          programs[0]->activate();
          draw_func();

          glEndQuery(GL_TIME_ELAPSED);

          unsigned int time;
          glGetQueryObjectuiv(gl_front_query_, GL_QUERY_RESULT, &time);
          set_time_taken((double)time / 1e9f);

          swap_queries_();
        };

        void write_imgui() {
          if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Render Passes")) {
              if (ImGui::TreeNode(name.c_str())) {
                ImGui::Text("Took %f seconds", time_taken_);
                ImGui::PlotLines("Time", time_cbuf_.data,
                    IM_ARRAYSIZE(time_cbuf_.data), time_cbuf_.offset, NULL,
                    0.0, 0.05);

                for (auto& p : programs)
                  p->write_imgui(); 

                framebuffer->write_imgui();
                ImGui::TreePop();
              }
              ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
          }
        }

        void set_time_taken(double t) {
          time_taken_ = t;
          time_cbuf_.add_point(t);
        }

        std::string name;

        std::shared_ptr<Framebuffer> framebuffer;
        std::vector<std::shared_ptr<ShaderProgram>> programs;
        std::function<void()> draw_func;

      private:
        void swap_queries_() {
          unsigned int tmp = gl_front_query_;
          gl_front_query_ = gl_back_query_;
          gl_back_query_ = tmp;
        }

        double time_taken_;

        unsigned int gl_front_query_;
        unsigned int gl_back_query_;

        CircularBuffer time_cbuf_;
    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_RENDER_PASS_H */
