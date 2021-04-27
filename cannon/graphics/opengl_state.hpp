#pragma once
#ifndef CANNON_GRAPHICS_OPENGL_STATE_H
#define CANNON_GRAPHICS_OPENGL_STATE_H 

/*!
 * \file cannon/graphics/opengl_state.hpp
 * File containing OpenGLState class definition.
 */

#include <iostream>

#include <glad/glad.h>

namespace cannon {
  namespace graphics {

    /*!
     * \brief Class encapsulating current OpenGL state.
     *
     * This class is primarily used to test postconditions of graphics package
     * methods that change OpenGL state.
     */
    struct OpenGLState {

      /*!
       * Constructor. Reads current OpenGL state, so objects should not be
       * created until just before the method to be tested.
       */
      OpenGLState() {
        glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&active_texture);
        glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&program);
        glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&texture);
        glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, (GLint*)&cubemap_texture);
        glGetIntegerv(GL_SAMPLER_BINDING, (GLint*)&sampler);
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*)&array_buffer);
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&vertex_array_object);
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, (GLint*)&element_buffer);
        glGetIntegerv(GL_DRAW_BUFFER, (GLint*)&draw_buffer);
        glGetIntegerv(GL_READ_BUFFER, (GLint*)&read_buffer);
        glGetIntegerv(GL_VIEWPORT, viewport); 
        glGetIntegerv(GL_SCISSOR_BOX, scissor_box);

        glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&blend_src_rgb);
        glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&blend_dst_rgb);
        glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&blend_src_alpha);
        glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&blend_dst_alpha);
        glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&blend_equation_rgb);
        glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&blend_equation_alpha);

        enable_blend = glIsEnabled(GL_BLEND);
        enable_cull_face = glIsEnabled(GL_CULL_FACE);
        enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
        enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);
        enable_primitive_restart = glIsEnabled(GL_PRIMITIVE_RESTART);
      }


      GLenum active_texture; //!< The current active texture
      GLuint program; //!< The current shader program
      GLuint texture; //!< The current bound texture
      GLuint cubemap_texture; //!< The current bound cubemap texture
      GLuint sampler; //!< The current texture sampler
      GLuint array_buffer; //!< The current bound array buffer
      GLuint element_buffer; //!< The current bound element buffer
      GLuint vertex_array_object; //!< The current bound vertex array object
      GLuint read_buffer; //!< The current bound read framebuffer
      GLuint draw_buffer; //!< The current bound read framebuffer
      GLint viewport[4]; //!< The current viewport
      GLint scissor_box[4]; //!< The current scissorbox

      GLenum blend_src_rgb; //!< The current blend source rgb setting
      GLenum blend_dst_rgb; //!< The current blend destination rgb setting
      GLenum blend_src_alpha; //!< The current blend source alpha setting
      GLenum blend_dst_alpha; //!< The current blend destination alpha setting
      GLenum blend_equation_rgb; //!< The current blend equation RGB mode
      GLenum blend_equation_alpha; //!< The current blend equation alpha mode
      
      GLboolean enable_blend; //!< Whether blending is enabled
      GLboolean enable_cull_face; //!< Whether face culling is enabled
      GLboolean enable_depth_test; //!< Whether depth testing is enabled
      GLboolean enable_scissor_test; //!< Whether scissor testing is enabled
      GLboolean enable_primitive_restart; //!< Whether primitive restart is enabled

    };

    /*!
     * Function to check whether two OpenGLStates are equal.
     */
    bool operator==(const OpenGLState& lhs, const OpenGLState& rhs); 

    /*!
     * Function to print this OpenGLState. Simply writes all member data.
     */
    std::ostream& operator<<(std::ostream& os, const OpenGLState& state);

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_OPENGL_STATE_H */
