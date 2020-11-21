#ifndef CANNON_GRAPHICS_DEFERRED_RENDERER_H
#define CANNON_GRAPHICS_DEFERRED_RENDERER_H 

#include <functional>

#include <cannon/graphics/viewer_3d.hpp>
#include <cannon/graphics/geometry/axes_hint.hpp>

namespace cannon {
  namespace graphics {
    
    class DeferredRenderer {
      public:
        DeferredRenderer() {
          setup_render_passes(); 
        }

        ~DeferredRenderer() {}

        // We do not care if this function changes OpenGL state, as its
        // conclusion means that the OpenGL context is destroyed.
        void render_loop(std::function<void()> f);

        void setup_render_passes();

        Viewer3D viewer;

      private:
        std::shared_ptr<ShaderProgram> gbuf_program_;
        std::shared_ptr<ShaderProgram> sdf_geom_program_;
        std::shared_ptr<ShaderProgram> shadow_program_;
        std::shared_ptr<ShaderProgram> sdf_shadow_program_;
        std::shared_ptr<ShaderProgram> ssao_program_;
        std::shared_ptr<ShaderProgram> ssao_blur_program_;
        std::shared_ptr<ShaderProgram> lighting_program_;
        std::shared_ptr<ShaderProgram> light_geom_program_;
        std::shared_ptr<ShaderProgram> hdr_program_;
        std::shared_ptr<ShaderProgram> axes_program_;

        std::shared_ptr<Framebuffer> gbuffer_fb_;
        std::shared_ptr<Framebuffer> shadow_fb_;
        std::shared_ptr<Framebuffer> ssao_fb_;
        std::shared_ptr<Framebuffer> ssao_blur_fb_;
        std::shared_ptr<Framebuffer> lighting_fb_;
        std::shared_ptr<Framebuffer> light_geom_fb_;
        std::shared_ptr<Framebuffer> hdr_fb_;
        std::shared_ptr<Framebuffer> axes_fb_;

        Matrix4f light_space_mat_;
        std::vector<Vector4f> ssao_kernel_;
        std::vector<float> ssao_noise_;
        geometry::AxesHint ah_;

    };

    float lerp(float a, float b, float f) {
      return a + f * (b - a);
    }

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_DEFERRED_RENDERER_H */
