#ifndef CANNON_GRAPHICS_VERTEX_SHADER_H
#define CANNON_GRAPHICS_VERTEX_SHADER_H

#include <string>
#include <streambuf>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cannon/log/registry.hpp>
#include <cannon/graphics/shader_program.hpp>

using namespace cannon::log;

namespace cannon {
  namespace graphics {

    class VertexShader {
      public:
        // Does not affect OpenGL state
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
          if (glfwGetCurrentContext() != NULL) {
            glDeleteShader(gl_shader_);
          }
        }

      private:
        unsigned int gl_shader_;

        friend class ShaderProgram;
    };

    // Free Functions
    VertexShader load_vertex_shader(const std::string& path);
    VertexShader load_vertex_shader(const std::string& main_path, const
        std::vector<std::string>& lib_paths);

    // Static shader sources
    static const std::string BASIC_VERTEX_SHADER = std::string("\
      #version 330 core\n\
      layout (location = 0) in vec3 aPos;\n\
      void main () {\n\
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n\
      }\
    ");

    static const std::string BASIC_VERTEX_SHADER_2D = std::string("\
      #version 330 core\n\
      layout (location = 0) in vec2 aPos;\n\
      uniform mat4 matrix;\n\
      void main () {\n\
        gl_Position = matrix * vec4(aPos.x, aPos.y, 0.0, 1.0);\n\
      }\
    ");



  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_VERTEX_SHADER_H */
