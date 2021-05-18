#pragma once
#ifndef CANNON_GRAPHICS_VERTEX_SHADER_H
#define CANNON_GRAPHICS_VERTEX_SHADER_H

/*!
 * \file cannon/graphics/vertex_shader.hpp
 * \brief File containing VertexShader class definition and utility functions
 * for loading shaders.
 */

#include <string>
#include <streambuf>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cannon/log/registry.hpp>

using namespace cannon::log;

namespace cannon {
  namespace graphics {

    /*!
     * \brief Class representing an OpenGL vertex shader.
     */
    class VertexShader {
      public:

        /*!
         * \brief Constructor taking a source string to compile into a vertex shader.
         */
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

        /*!
         * \brief Destructor. Cleans up OpenGL shader object if possible.
         */
        ~VertexShader() {
          if (glfwGetCurrentContext() != NULL) {
            glDeleteShader(gl_shader_);
          }
        }

      private:
        unsigned int gl_shader_; //!< OpenGL shader object

        friend class ShaderProgram;
    };

    // Free Functions
    
    /*!
     * \brief Load a vertex shader from an input filename.
     *
     * \param path Path of the shader to load.
     *
     * \returns The loaded shader.
     */
    VertexShader load_vertex_shader(const std::string& path);

    /*!
     * \brief Load a vertex shader from an input filename and a vector of
     * shader library files.
     *
     * \param main_path Path of shader file containing main function.
     * \param lib_paths Paths of shader files that should be compiled with the main file.
     *
     * \returns The loaded combined shader.
     */
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
