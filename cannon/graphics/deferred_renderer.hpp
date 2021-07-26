#pragma once
#ifndef CANNON_GRAPHICS_DEFERRED_RENDERER_H
#define CANNON_GRAPHICS_DEFERRED_RENDERER_H 

/*!
 * \file cannon/graphics/deferred_renderer.hpp
 * \brief File containing DeferredRenderer class definition and cannon::graphics::lerp
 * function definition.
 */

#include <functional>

#include <cannon/graphics/geometry/axes_hint.hpp>

#include <cannon/utils/class_forward.hpp>

namespace cannon {
  namespace graphics {

    CANNON_CLASS_FORWARD(Framebuffer);
    CANNON_CLASS_FORWARD(ShaderProgram);
    CANNON_CLASS_FORWARD(Viewer3D);
    
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
        DeferredRenderer();

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

        Viewer3DPtr viewer; //!< Internal viewer. DeferredRenderer is really a wrapper around this.

      private:
        ShaderProgramPtr gbuf_program_; //!< Shader used to draw to the gbuffer
        ShaderProgramPtr sdf_geom_program_; //!< Shader used to draw SDF geometry
        ShaderProgramPtr shadow_depth_program_; //!< Shader used to draw regular geometry shadow depth
        ShaderProgramPtr sdf_shadow_depth_program_; //!< Shader used to draw SDF geometry shadow depth
        ShaderProgramPtr shadow_program_; //!< Shader used to populate shadow buffer
        ShaderProgramPtr shadow_blur_program_; //!< Shader used to blur shadow information
        ShaderProgramPtr ssao_program_; //!< Shader that implements SSAO. See https://learnopengl.com/Advanced-Lighting/SSAO
        ShaderProgramPtr ssao_blur_program_; //!< Shader that blurs SSAO information
        ShaderProgramPtr lighting_program_; //!< Shader that does deferred lighting
        ShaderProgramPtr light_geom_program_; //!< Shader that renderers point light geometry
        ShaderProgramPtr hdr_program_; //!< Shader that does HDR tone mapping
        ShaderProgramPtr axes_program_; //!< Shader that displays a small axis hint for 3D navigation

        FramebufferPtr gbuffer_fb_; //!< GBuffer framebuffer
        FramebufferPtr shadow_depth_fb_; //!< Shadow depth information framebuffer
        FramebufferPtr shadow_fb_; //!< Shadow information framebuffer
        FramebufferPtr shadow_blur_1_fb_; //!< Horizontal shadow blur framebuffer
        FramebufferPtr shadow_blur_2_fb_; //!< Vertical shadow blur framebuffer
        FramebufferPtr ssao_fb_; //!< SSAO framebuffer
        FramebufferPtr ssao_blur_fb_; //!< SSAO blur framebuffer
        FramebufferPtr lighting_fb_; //!< Deferred lighting framebuffer
        FramebufferPtr light_geom_fb_; //!< Point light geometry framebuffer
        FramebufferPtr hdr_fb_; //!< HDR tone mapping framebuffer
        FramebufferPtr axes_fb_; //!< Axis hint drawing framebuffer

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
