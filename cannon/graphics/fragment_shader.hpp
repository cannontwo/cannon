#ifndef CANNON_GRAPHICS_FRAGMENT_SHADER_H
#define CANNON_GRAPHICS_FRAGMENT_SHADER_H

#include <string>
#include <stdexcept>
#include <glad/glad.h>

#include <cannon/log/registry.hpp>
#include <cannon/graphics/shader_program.hpp>

using namespace cannon::log;

namespace cannon {
  namespace graphics {

    class FragmentShader {
      public:
        FragmentShader(const char** source) {
          gl_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
          if (gl_shader_ == 0) 
            throw std::runtime_error("Could not create fragment shader");
          
          glShaderSource(gl_shader_, 1, source, NULL);
          glCompileShader(gl_shader_);
          
          // Check for shader compilation error
          int success;
          char info[512];
          glGetShaderiv(gl_shader_, GL_COMPILE_STATUS, &success);
          if (!success) {
            glGetShaderInfoLog(gl_shader_, 512, NULL, info);
            log_error(info);
            throw std::runtime_error("Could not compile fragment shader source.");
          }
        }

        ~FragmentShader() {
          glDeleteShader(gl_shader_);
        }

      private:
        unsigned int gl_shader_;

        friend class ShaderProgram;
    };

    // Static shader sources
    static const std::string BASIC_FRAGMENT_SHADER = std::string("\
      #version 330 core\n\
      out vec4 FragColor;\n\
      void main() {\n\
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n\
      }\
    ");

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_FRAGMENT_SHADER_H */
