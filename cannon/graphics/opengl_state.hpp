#ifndef CANNON_GRAPHICS_OPENGL_STATE_H
#define CANNON_GRAPHICS_OPENGL_STATE_H 

#include <iostream>

#include <glad/glad.h>

namespace cannon {
  namespace graphics {

    struct OpenGLState {

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


      GLenum active_texture;
      GLuint program;
      GLuint texture;
      GLuint cubemap_texture;
      GLuint sampler;
      GLuint array_buffer;
      GLuint element_buffer;
      GLuint vertex_array_object;
      GLuint read_buffer;
      GLuint draw_buffer;
      GLint viewport[4];
      GLint scissor_box[4];

      GLenum blend_src_rgb;
      GLenum blend_dst_rgb;
      GLenum blend_src_alpha;
      GLenum blend_dst_alpha;
      GLenum blend_equation_rgb;
      GLenum blend_equation_alpha;
      
      GLboolean enable_blend;
      GLboolean enable_cull_face;
      GLboolean enable_depth_test;
      GLboolean enable_scissor_test;
      GLboolean enable_primitive_restart;

    };

    bool operator==(const OpenGLState& lhs, const OpenGLState& rhs);
    std::ostream& operator<<(std::ostream& os, const OpenGLState& state);

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_OPENGL_STATE_H */
