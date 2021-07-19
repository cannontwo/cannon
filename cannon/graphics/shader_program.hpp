#pragma once
#ifndef CANNON_GRAPHICS_SHADER_PROGRAM_H
#define CANNON_GRAPHICS_SHADER_PROGRAM_H 

/*!
 * \file cannon/graphics/shader_program.hpp
 * \brief File containing ShaderProgram class definition.
 */

#include <Eigen/Dense>
#include <cannon/utils/class_forward.hpp>

using namespace Eigen;

namespace cannon {
  namespace graphics {

    CANNON_CLASS_FORWARD(VertexShader);
    CANNON_CLASS_FORWARD(FragmentShader);

    /*!
     * \brief Class representing a shader program.
     *
     * This class abstracts the loading and compilation of shader programs for
     * use with other graphics classes. It includes both the vertex and fragment shaders.
     */
    class ShaderProgram {
      public:

        /*!
         * Constructor taking a name and whether to initialize OpenGL variables for this shader.
         *
         * Does not affect OpenGL state.
         */
        ShaderProgram(const std::string& name = "shader", bool do_init=true) : name_(name) {
          if (do_init) {
            init();
          }
        }

        /*!
         * Copy constructor.
         */
        ShaderProgram(ShaderProgram& s) = delete;

        /*!
         * Move constructor.
         */
        ShaderProgram(ShaderProgram&& s) :
          gl_shader_program_(s.gl_shader_program_),
          initialized_(s.initialized_), name_(s.name_) {
            s.gl_shader_program_ = -1;
          }

        /*!
         * Destructor. Frees OpenGL resources.
         */
        ~ShaderProgram();

        // TODO Replace this with explicit overloads
        
        /*!
         * Attach a vertex shader to this shader program.
         *
         * \param shader The shader to attach.
         */
        void attach_shader(const VertexShader& shader);

        /*!
         * Attach a fragment shader to this shader program.
         *
         * \param shader The shader to attach.
         */
        void attach_shader(const FragmentShader& shader);

        /*!
         * Attach a vertex shader to this shader program, given its source and
         * any shader libraries to attach.
         *
         * \param v_src The vertex shader source path.
         * \param v_libs Locations of shader library files to load.
         *
         * Does not affect OpenGL state.
         */
        void attach_vertex_shader(const std::string& v_src, const
            std::vector<std::string>& v_libs = {});

        /*!
         * Attach a fragment shader to this shader program, given its source
         * and any shader libraries to attach.
         *
         * \param f_src The fragment shader source path.
         * \param f_libs Locations of shader library files to load.
         *
         * Does not affect OpenGL state.
         */
        void attach_fragment_shader(const std::string& f_src, const
            std::vector<std::string>& f_libs = {});

        /*!
         * Initialize this shader program by allocating OpenGL resources.
         * 
         * Does not affect OpenGL state.
         */
        void init();

        /*!
         * Reload this shader program from sources. 
         *
         * Does not affect OpenGL state.
         */
        void reload();

        /*!
         * Link this shader program. 
         *
         * Does not affect OpenGL state.
         */
        void link();

        /*!
         * Activate this shader program for drawing.
         *
         * Sets GL_CURRENT_PROGRAM to gl_shader_program_.
         */
        void activate();

        /*!
         * Deactivate this shader program.
         *
         * Sets GL_CURRENT_PROGRAM to 0.
         */
        void deactivate();

        /*!
         * Set an integer uniform for this shader program.
         *
         * \param name The name of the uniform.
         * \param value Value to set the uniform to.
         * \param verbose Whether to report to console if uniform not found.
         *
         * Does not affect OpenGL state.
         */
        void set_uniform(const std::string& name, int value, bool verbose=false);

        /*!
         * Set a float uniform for this shader program.
         *
         * \param name The name of the uniform.
         * \param value Value to set the uniform to.
         * \param verbose Whether to report to console if uniform not found.
         *
         * Does not affect OpenGL state.
         */
        void set_uniform(const std::string& name, float value, bool verbose=false);

        /*!
         * Set a Vector4f uniform for this shader program.
         *
         * \param name The name of the uniform.
         * \param value Value to set the uniform to.
         * \param verbose Whether to report to console if uniform not found.
         *
         * Does not affect OpenGL state.
         */
        void set_uniform(const std::string& name, Vector4f value, bool verbose=false);

        /*!
         * Set a Matrix4f uniform for this shader program.
         *
         * \param name The name of the uniform.
         * \param value Value to set the uniform to.
         * \param verbose Whether to report to console if uniform not found.
         *
         * Does not affect OpenGL state.
         */
        void set_uniform(const std::string& name, Matrix4f value, bool verbose=false);

        /*!
         * Write ImGui controls for this shader program. 
         *
         * Does not affect OpenGL state.
         */
        void write_imgui();
        
      private:
        /*!
         * Utility method to get the location of a uniform in this shader program.
         *
         * \param name The name of the uniform. 
         * \param verbose Whether to report to console if uniform not found.
         *
         * \returns The location of the queried uniform.
         */
        int get_uniform_loc_(const std::string& name, bool verbose=true);

        unsigned int gl_shader_program_; //!< OpenGL shader program resource
        bool initialized_ = false; //!< Whether this shader program has been initialized

        std::string name_; //!< The name of this shader program
        std::string v_src_; //!< Path to vertex shader source
        std::vector<std::string> v_libs_; //!< Paths to vertex shader libraries
        std::string f_src_; //!< Path to fragment shader source
        std::vector<std::string> f_libs_; //!< Path to fragment shader libraries

    };

  } // namespace graphics
} // namespace cannon


#endif /* ifndef CANNON_GRAPHICS_SHADER_PROGRAM_H */
