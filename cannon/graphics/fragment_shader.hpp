#pragma once
#ifndef CANNON_GRAPHICS_FRAGMENT_SHADER_H
#define CANNON_GRAPHICS_FRAGMENT_SHADER_H

/*!
 * \file cannon/graphics/fragment_shader.hpp
 * \brief File containing FragmentShader class definition and free functions relating
 * to fragment shaders for convenience.
 */

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace cannon {
  namespace graphics {

    /*!
     * \brief Class representing a compiled OpenGL fragment shader.
     *
     * This class  is very similar to cannon::graphics::VertexShader, but
     * slightly different OpenGL calls are needed to compile the two kinds of
     * shaders. It is primarily useful in the context of a
     * cannon::graphics::ShaderProgram.
     *
     * \sa cannon::graphics::VertexShader, cannon::graphics::ShaderProgram.
     */
    class FragmentShader {
      public:

        /*!
         * Constructor taking the shader source code to be compiled into a
         * fragment shader. The source should be in GLSL.
         *
         * Does not affect OpenGL state.
         *
         * \param source A null-terminated C-string containing the shader source code.
         */
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
            throw std::runtime_error("Could not compile fragment shader source.");
          }
        }

        /*!
         * Destructor. Frees allocated OpenGL shader object.
         *
         * Does not affect OpenGL state.
         */
        ~FragmentShader() {
          if (glfwGetCurrentContext() != NULL) {
            glDeleteShader(gl_shader_);
          }
        }

      private:
        unsigned int gl_shader_; //!< The OpenGL shader object.

        friend class ShaderProgram;
    };

    // Free Functions
    
    /*!
     * Utility free function that reads shader source from a file path.
     *
     * \param path The path to read fragment shader source from.
     *
     * \returns A FragmentShader object constructed from the source code in the file.
     */
    FragmentShader load_fragment_shader(const std::string& path);

    /*!
     * Utility free function that reads main shader source code from a file
     * path, then compiles it together with a number of other, "library" GLSL
     * files containing utility code that can be referenced from the main GLSL
     * file.
     *
     * For an example of the usage of this function, see
     * cannon::graphics::DeferredRenderer::setup_render_passes.
     *
     * \param main_path Path to read the main fragment shader source from.
     * \param lib_paths Vector of file paths to read utility shader source from.
     *
     * \returns A FragmentShader object constructed from the combined GLSL source code.
     */
    FragmentShader load_fragment_shader(const std::string& main_path, const
        std::vector<std::string>& lib_paths);

    // Static shader sources
    
    /*!
     * Simple fragment shader for testing.
     */
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
