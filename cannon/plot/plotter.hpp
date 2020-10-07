#ifndef CANNON_PLOT_PLOTTER_H
#define CANNON_PLOT_PLOTTER_H 

#include <string>
#include <vector>

#include <cannon/graphics/window.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>
#include <cannon/plot/scatter.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::graphics;
using namespace cannon::log;

namespace cannon {
  namespace plot {

    class Plotter {
      public:
        Plotter() : w_(), program_() {
          w_.set_clear_color(Vector4f(1.0f, 1.0f, 1.0f, 1.0f));

          const char *v_src = BASIC_VERTEX_SHADER_2D.c_str();
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

        void plot_points(MatrixX2f points, Vector4f color={0.0, 0.0, 0.0, 1.0}, float size=15.0);

      private:
        Window w_;
        ShaderProgram program_;
        std::vector<Scatter> scatter_plots_;
    };

  } // namespace plot
} // namespace cannon

#endif /* ifndef CANNON_PLOT_PLOTTER_H */
