#ifndef CANNON_GRAPHICS_VERTEX_SHADER_H
#define CANNON_GRAPHICS_VERTEX_SHADER_H

#include <string>
#include <stdexcept>
#include <glad/glad.h>

#include <cannon/log/registry.hpp>
#include <cannon/graphics/shader_program.hpp>

using namespace cannon::log;

namespace cannon {
  namespace graphics {

    class VertexShader {
      public:
        VertexShader(const char** source) {
          gl_shader_ = glCreateShader(GL_VERTEX_SHADER);
          if (gl_shader_ == 0) 
            throw std::runtime_error("Could not create vertex shader");
          
          glShaderSource(gl_shader_, 1, source, NULL);
          glCompileShader(gl_shader_);
          
          // Check for shader compilation error
          int success;
          char info[512];
          glGetShaderiv(gl_shader_, GL_COMPILE_STATUS, &success);
          if (!success) {
            glGetShaderInfoLog(gl_shader_, 512, NULL, info);
            log_error(info);
            throw std::runtime_error("Could not compile vertex shader source.");
          }
        }

        ~VertexShader() {
          glDeleteShader(gl_shader_);
        }

      private:
        unsigned int gl_shader_;

        friend class ShaderProgram;
    };

    // Static shader sources
    static const std::string BASIC_VERTEX_SHADER = std::string("\
      #version 330 core\n\
      layout (location = 0) in vec3 aPos;\n\
      void main () {\n\
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n\
      }\
    ");

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_VERTEX_SHADER_H */
