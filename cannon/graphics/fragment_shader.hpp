#ifndef CANNON_GRAPHICS_FRAGMENT_SHADER_H
#define CANNON_GRAPHICS_FRAGMENT_SHADER_H

#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
          if (glfwGetCurrentContext() != NULL) {
            glDeleteShader(gl_shader_);
          }
        }

      private:
        unsigned int gl_shader_;

        friend class ShaderProgram;
    };

    // Free Functions
    FragmentShader load_fragment_shader(const std::string& path);
    FragmentShader load_fragment_shader(const std::string& main_path, const
        std::vector<std::string>& lib_paths);

    // Static shader sources
    static const std::string BASIC_FRAGMENT_SHADER = std::string("\
      #version 330 core\n\
      out vec4 FragColor;\n\
      uniform vec4 uColor;\n\
      void main() {\n\
        FragColor = uColor;\n\
      }\
    ");

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_FRAGMENT_SHADER_H */
