#ifndef CANNON_PLOT_PLOTTER_H
#define CANNON_PLOT_PLOTTER_H 

#include <string>

#include <cannon/graphics/window.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>

using namespace cannon::graphics;

namespace cannon {
  namespace plot {

    class Plotter {
      public:
        Plotter() : w_(), program_() {
          w_.set_clear_color(Vector4f(1.0f, 1.0f, 1.0f, 1.0f));

          const char *v_src = BASIC_VERTEX_SHADER.c_str();
          const char *f_src = BASIC_FRAGMENT_SHADER.c_str();
          log_info(v_src);
          log_info(f_src);

          VertexShader v(&v_src);
          FragmentShader f(&f_src);

          program_.attach_shader(v);
          program_.attach_shader(f);
          program_.link();
          program_.activate();
        }

        void render();

      private:
        Window w_;
        ShaderProgram program_;
    };

  } // namespace plot
} // namespace cannon

#endif /* ifndef CANNON_PLOT_PLOTTER_H */
