#pragma once
#ifndef CANNON_GRAPHICS_FRAMEBUFFER_H
#define CANNON_GRAPHICS_FRAMEBUFFER_H 

/*!
 * \file cannon/graphics/framebuffer.hpp
 * \brief File containing Framebuffer class definition.
 */

#include <stdexcept>
#include <memory>
#include <queue>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/utils/class_forward.hpp>

namespace cannon {
  namespace graphics {

    CANNON_CLASS_FORWARD(Texture);

    namespace geometry {
      CANNON_CLASS_FORWARD(ScreenQuad);
    }

    /*!
     * \brief Class representing an OpenGL framebuffer.
     *
     * This class is primarily used in the deferred rendering pipeline, where a
     * series of renders to different framebuffers is chained together.
     * Generally, this class can be used at any stage of OpenGL
     * rendering, either to read from or write to.
     *
     * \sa cannon::graphics::DeferredRenderer
     */
    class Framebuffer {
      public:

        /*!
         * Constructor taking the width, height, and name of this framebuffer.
         * Color and depth attachments are created automatically.
         *
         * \param width The width for the constructed framebuffer.
         * \param height The height for the constructed framebuffer.
         * \param name The name for the constructed framebuffer. Primarily for ImGui display.
         */
        Framebuffer(int width=800, int height=600, const std::string& name="");

        /*!
         * Constructor taking a vector of attachments, width, height, and name
         * of this framebuffer. This constructor is useful when creating a
         * framebuffer from existing color or depth textures.
         *
         * \param attachments The attachments for the constructed framebuffer.
         * \param width The width of the constructed framebuffer.
         * \param height The height of the constructed framebuffer.
         * \param name The name for the constructed framebuffer. Primarily for ImGui display.
         */
        Framebuffer(std::vector<TexturePtr> attachments, int width=800, int
            height=600, const std::string& name="");

        /*!
         * Destructor, which also cleans up OpenGL resources.
         */
        ~Framebuffer() {
          if (glfwGetCurrentContext() != NULL) {
            glDeleteFramebuffers(1, &gl_framebuffer_);
          }
        }

        /*!
         * Method to bind this framebuffer for both reading and writing.
         * 
         * Sets GL_READ_FRAMEBUFFER to gl_framebuffer_, and GL_DRAW_FRAMEBUFFER
         * to gl_framebuffer_.
         */
        void bind(); 

        /*!
         * Method to bind this framebuffer for reading.
         *
         * Sets GL_READ_FRAMEBUFFER to gl_framebuffer_.
         */
        void bind_read();

        /*!
         * Method to unbind this framebuffer for reading.
         *
         * Sets GL_READ_FRAMEBUFFER to 0.
         */
        void unbind_read();

        /*!
         * Method to bind this framebuffer for drawing.
         *
         * Sets GL_DRAW_FRAMEBUFFER to gl_framebuffer_.
         */
        void bind_draw();

        /*!
         * Method to unbind this framebuffer for drawing.
         *
         * Sets GL_DRAW_FRAMEBUFFER to 0.
         */
        void unbind_draw();

        /*!
         * Method to unbind this framebuffer from both reading and writing.
         *
         * Sets GL_READ_FRAMEBUFFER to 0 and GL_DRAW_FRAMEBUFFER to 0.
         */
        void unbind();

        /*!
         * Method to draw the current contents of this framebuffer to the
         * OpenGL backbuffer.
         *
         * Sets GL_READ_FRAMEBUFFER to gl_framebuffer_ and GL_DRAW_FRAMEBUFFER
         * to gl_framebuffer_.
         */
        void display();

        /*!
         * Method to draw a quad of the same size as this framebuffer, textured
         * with the contents of this framebuffer's 0th color attachment.
         *
         * Does not affect OpenGL state.
         */
        void draw_quad();

        /*!
         * Method to draw a quad of the same size as this framebuffer, textured
         * with the contents of the idxth color attachment of this framebuffer.
         *
         * Does not affect OpenGL state.
         */
        void draw_quad(int idx);

        /*!
         * Method to attach a color attachment to this framebuffer. 
         *
         * Does not affect OpenGL state.
         */
        void attach_tex(TexturePtr tex);

        /*!
         * Method to resize this framebuffer. Also resizes all of this
         * framebuffer's color attachments.
         *
         * Does not affect OpenGL state.
         */
        void resize(int w, int h);
        
        /*!
         * Method to write this framebuffer's ImGui controls.
         *
         * Does not affect OpenGL state.
         */
        void write_imgui();

        int width; //!< The width of this framebuffer.
        int height; //!< The height of this framebuffer.

        geometry::ScreenQuadPtr quad; //!< The quad used to draw this framebuffer.
        std::string name; //!< The name of this framebuffer.
        std::deque<TexturePtr> color_attachments; //!< Color attachments storing the actual information of this framebuffer.

      private:
        
        /*!
         * Method to generate a combined depth/stencil buffer.
         */
        void generate_depth_stencil_buffer_();

        unsigned int gl_framebuffer_; //!< OpenGL object representing this framebuffer.
        unsigned int gl_depth_stencil_rb_; //!< OpenGL object representing this framebuffer's depth/stencil buffer.

        unsigned int max_color_attachments_ = 8; //!< The maximum number of color attachments supported by this framebuffer.

    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_FRAMEBUFFER_H */
