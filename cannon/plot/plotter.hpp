#ifndef CANNON_PLOT_PLOTTER_H
#define CANNON_PLOT_PLOTTER_H 

#include <string>
#include <vector>

#include <cannon/graphics/window.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>
#include <cannon/plot/scatter.hpp>
#include <cannon/plot/axes.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::graphics;
using namespace cannon::log;

namespace cannon {
  namespace plot {

    class Plotter {
      public:
        Plotter() : w_(), point_program_(new ShaderProgram) {
          w_.set_clear_color(Vector4f(1.0f, 1.0f, 1.0f, 1.0f));

          VertexShader v = load_vertex_shader("shaders/basic_2d_pass_pos.vert");
          FragmentShader f = load_fragment_shader("shaders/uniform_color_circle.frag");

          point_program_->attach_shader(v);
          point_program_->attach_shader(f);
          point_program_->link();
          point_program_->activate();
        }

        void render();
        void save(const std::string &path);

        void plot_points(MatrixX2f points, Vector4f color={0.0, 0.0, 0.0, 1.0}, float size=15.0);

      private:
        void draw_pass();

        Window w_;
        std::shared_ptr<ShaderProgram> point_program_;
        Axes axes_;
        std::vector<Scatter> scatter_plots_;
    };

  } // namespace plot
} // namespace cannon

#endif /* ifndef CANNON_PLOT_PLOTTER_H */
