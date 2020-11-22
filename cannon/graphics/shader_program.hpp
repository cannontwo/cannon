#ifndef CANNON_GRAPHICS_SHADER_PROGRAM_H
#define CANNON_GRAPHICS_SHADER_PROGRAM_H 

#include <glad/glad.h>
#include <stdexcept>
#include <Eigen/Dense>
#include <imgui.h>
#include <imgui_stdlib.h>

#include <cannon/log/registry.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>

using namespace cannon::log;
using namespace Eigen;

namespace cannon {
  namespace graphics {

    class ShaderProgram {
      public:
        ShaderProgram(const std::string& name = "shader", bool do_init=true) : name_(name) {
          if (do_init) {
            init();
          }
        }

        ShaderProgram(ShaderProgram& s) = delete;

        ShaderProgram(ShaderProgram&& s) :
          gl_shader_program_(s.gl_shader_program_),
          initialized_(s.initialized_), name_(s.name_) {
            s.gl_shader_program_ = -1;
          }

        ~ShaderProgram() {
          glDeleteProgram(gl_shader_program_);
        }

        template <typename T>
        void attach_shader(const T& shader) {
          glAttachShader(gl_shader_program_, shader.gl_shader_);
        }

        void attach_vertex_shader(const std::string& v_src, const
            std::vector<std::string>& v_libs = {});
        void attach_fragment_shader(const std::string& f_src, const
            std::vector<std::string>& f_libs = {});

        void init() {
          gl_shader_program_ = glCreateProgram();
          initialized_ = true;
        }

        void reload();

        void link();
        void activate();
        void deactivate();
        void set_uniform(const std::string& name, int value, bool verbose=false);
        void set_uniform(const std::string& name, float value, bool verbose=false);
        void set_uniform(const std::string& name, Vector4f value, bool verbose=false);
        void set_uniform(const std::string& name, Matrix4f value, bool verbose=false);

        void write_imgui();
        
      private:
        int get_uniform_loc_(const std::string& name, bool verbose=true);

        unsigned int gl_shader_program_;
        bool initialized_ = false;

        std::string name_;
        std::string v_src_;
        std::vector<std::string> v_libs_;
        std::string f_src_;
        std::vector<std::string> f_libs_;

    };

  } // namespace graphics
} // namespace cannon


#endif /* ifndef CANNON_GRAPHICS_SHADER_PROGRAM_H */
