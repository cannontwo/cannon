#ifndef CANNON_GRAPHICS_RENDER_PASS_H
#define CANNON_GRAPHICS_RENDER_PASS_H 

#include <memory>

#include <cannon/graphics/framebuffer.hpp>
#include <cannon/graphics/shader_program.hpp>

namespace cannon {
  namespace graphics {

    template <typename F>
    class RenderPass {
      public:
        RenderPass(std::shared_ptr<Framebuffer> fb,
            std::shared_ptr<ShaderProgram> p, F f) : framebuffer_(fb),
        program_(p), draw_func_(f) {}

        void run() {
          framebuffer_->bind();
          program_->activate();
          draw_func_();
        };

      private:
        std::shared_ptr<Framebuffer> framebuffer_;
        std::shared_ptr<ShaderProgram> program_;
        F draw_func_;

    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_RENDER_PASS_H */
