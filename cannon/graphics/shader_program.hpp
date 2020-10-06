#ifndef CANNON_GRAPHICS_SHADER_PROGRAM_H
#define CANNON_GRAPHICS_SHADER_PROGRAM_H 

#include <glad/glad.h>
#include <stdexcept>
#include <Eigen/Dense>

#include <cannon/log/registry.hpp>

using namespace cannon::log;
using namespace Eigen;

namespace cannon {
  namespace graphics {

    class ShaderProgram {
      public:
        ShaderProgram() {
          gl_shader_program_ = glCreateProgram();
        }

        ~ShaderProgram() {
          glDeleteProgram(gl_shader_program_);
        }

        template <typename T>
        void attach_shader(const T& shader) {
          glAttachShader(gl_shader_program_, shader.gl_shader_);
        }

        void link();
        void activate();
        void set_uniform(const std::string& name, int value);
        void set_uniform(const std::string& name, Vector4f value);
        void set_uniform(const std::string& name, Matrix4f value);

      private:
        int get_uniform_loc_(const std::string& name);

        unsigned int gl_shader_program_;

    };

  } // namespace graphics
} // namespace cannon


#endif /* ifndef CANNON_GRAPHICS_SHADER_PROGRAM_H */
