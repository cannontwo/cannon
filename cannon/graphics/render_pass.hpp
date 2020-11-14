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
          name(name), framebuffer(fb), program(p), draw_func(f) {}

        RenderPass(RenderPass& o) : name(o.name), framebuffer(o.framebuffer),
        program(o.program), draw_func(o.draw_func) {}

        RenderPass(RenderPass&& o) : name(o.name), framebuffer(o.framebuffer),
        program(o.program), draw_func(o.draw_func) {}

        void run() {
          framebuffer->bind();
          program->activate();
          draw_func();
        };

        void write_imgui() {
          if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Render Passes")) {
              if (ImGui::TreeNode(name.c_str())) {
                ImGui::Text("Took %f seconds", time_taken_);
                ImGui::PlotLines("Time", time_cbuf_.data,
                    IM_ARRAYSIZE(time_cbuf_.data), time_cbuf_.offset, NULL,
                    0.0, 0.05);
                program->write_imgui(); 
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
        std::shared_ptr<ShaderProgram> program;
        std::function<void()> draw_func;

      private:
        double time_taken_;

        CircularBuffer time_cbuf_;
    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_RENDER_PASS_H */
