#ifndef CANNON_GRAPHICS_DEFERRED_RENDERER_H
#define CANNON_GRAPHICS_DEFERRED_RENDERER_H 

/*!
 * \file cannon/graphics/deferred_renderer.hpp
 * File containing DeferredRenderer class definition and cannon::graphics::lerp
 * function definition.
 */

#include <functional>

#include <cannon/graphics/viewer_3d.hpp>
#include <cannon/graphics/geometry/axes_hint.hpp>

namespace cannon {
  namespace graphics {
    
    /*!
     * \brief Class representing a deferred renderer for OpenGL.
     *
     * This class is effectively just a 3DViewer with pre-specified render
     * passes defining a deferred shading pipeline. Additional render passes
     * can be added to this class, but the intention is that this renderer can
     * be used to draw 3D geometry without any additional rendering
     * configuration. Adapted from
     * https://learnopengl.com/Advanced-Lighting/Deferred-Shading .
     *
     * \sa cannon::graphics::Viewer3D
     */
    class DeferredRenderer {
      public:

        /*!
         * Default constructor, which sets up render passes.
         */
        DeferredRenderer() {
          setup_render_passes(); 
        }

        /*!
         * Destructor. No cleanup is necessary because all graphics objects
         * free their held OpenGL resources on deletion.
         */
        ~DeferredRenderer() {}


        /*!
         * Method representing the main render loop of this renderer. Blocks
         * until the OpenGL window is closed. The passed function will be
         * called on each rendered frame, and provides a way of updating the
         * state of rendered objects (i.e., simulating physics or otherwise
         * animating graphics state).
         *
         * We do not care if this function changes OpenGL state, as its
         * termination means that the OpenGL context is destroyed.
         *
         * \param f The function to be called on each frame. 
         */
        void render_loop(std::function<void()> f);

        /*!
         * Method which handles initialization of all render passes run by this
         * renderer.
         */
        void setup_render_passes();

        Viewer3D viewer; //!< Internal viewer. DeferredRenderer is really a wrapper around this.

      private:
        std::shared_ptr<ShaderProgram> gbuf_program_; //!< Shader used to draw to the gbuffer
        std::shared_ptr<ShaderProgram> sdf_geom_program_; //!< Shader used to draw SDF geometry
        std::shared_ptr<ShaderProgram> shadow_depth_program_; //!< Shader used to draw regular geometry shadow depth
        std::shared_ptr<ShaderProgram> sdf_shadow_depth_program_; //!< Shader used to draw SDF geometry shadow depth
        std::shared_ptr<ShaderProgram> shadow_program_; //!< Shader used to populate shadow buffer
        std::shared_ptr<ShaderProgram> shadow_blur_program_; //!< Shader used to blur shadow information
        std::shared_ptr<ShaderProgram> ssao_program_; //!< Shader that implements SSAO. See https://learnopengl.com/Advanced-Lighting/SSAO
        std::shared_ptr<ShaderProgram> ssao_blur_program_; //!< Shader that blurs SSAO information
        std::shared_ptr<ShaderProgram> lighting_program_; //!< Shader that does deferred lighting
        std::shared_ptr<ShaderProgram> light_geom_program_; //!< Shader that renderers point light geometry
        std::shared_ptr<ShaderProgram> hdr_program_; //!< Shader that does HDR tone mapping
        std::shared_ptr<ShaderProgram> axes_program_; //!< Shader that displays a small axis hint for 3D navigation

        std::shared_ptr<Framebuffer> gbuffer_fb_; //!< GBuffer framebuffer
        std::shared_ptr<Framebuffer> shadow_depth_fb_; //!< Shadow depth information framebuffer
        std::shared_ptr<Framebuffer> shadow_fb_; //!< Shadow information framebuffer
        std::shared_ptr<Framebuffer> shadow_blur_1_fb_; //!< Horizontal shadow blur framebuffer
        std::shared_ptr<Framebuffer> shadow_blur_2_fb_; //!< Vertical shadow blur framebuffer
        std::shared_ptr<Framebuffer> ssao_fb_; //!< SSAO framebuffer
        std::shared_ptr<Framebuffer> ssao_blur_fb_; //!< SSAO blur framebuffer
        std::shared_ptr<Framebuffer> lighting_fb_; //!< Deferred lighting framebuffer
        std::shared_ptr<Framebuffer> light_geom_fb_; //!< Point light geometry framebuffer
        std::shared_ptr<Framebuffer> hdr_fb_; //!< HDR tone mapping framebuffer
        std::shared_ptr<Framebuffer> axes_fb_; //!< Axis hint drawing framebuffer

        Matrix4f light_space_mat_; //!< Light perspective matrix used for shadow mapping
        std::vector<Vector4f> ssao_kernel_; //!< Kernel used to blur SSAO information
        std::vector<float> ssao_noise_; //!< Noise used to compute SSAO
        geometry::AxesHint ah_; //!< Axis hint geometry

    };

    /*!
     * Function implementing linear interpolation.
     *
     * \param a The start of the interpolation.
     * \param b The end of the interpolation.
     * \param f Amount to interpolate between a and b.
     *
     * \returns a + f*(b - a)
     */
    float lerp(float a, float b, float f);

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_DEFERRED_RENDERER_H */
