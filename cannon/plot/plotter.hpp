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
#include <cannon/plot/line.hpp>

using namespace cannon::graphics;
using namespace cannon::log;

namespace cannon {
  namespace plot {

    class Scatter;
    class Line;

    class Plotter {
      public:
        Plotter() : w_(), point_program_(new ShaderProgram), line_program_(new
            ShaderProgram), axes_(2.0f / (float)w_.height) {
          w_.set_clear_color(Vector4f(1.0f, 1.0f, 1.0f, 1.0f));

          // Set up point shader
          VertexShader v = load_vertex_shader("shaders/basic_2d_pass_pos.vert");
          FragmentShader f = load_fragment_shader("shaders/uniform_color_circle.frag");

          point_program_->attach_shader(v);
          point_program_->attach_shader(f);
          point_program_->link();
          point_program_->activate();

          // Set up line shader
          const char *fl_src = BASIC_FRAGMENT_SHADER.c_str();
          VertexShader vl = load_vertex_shader("shaders/basic_2d_pass_pos.vert");
          FragmentShader fl = FragmentShader(&fl_src);

          line_program_->attach_shader(vl);
          line_program_->attach_shader(fl);
          line_program_->link();
          line_program_->activate();
        }

        void render();

        // Variant for rendering an animated plot
        template <typename F>
        void render(F f) {
          w_.render_loop([this, f] {draw_pass();f();});
        }

        void save(const std::string &path);
        void set_xlim(float low, float high);
        void set_ylim(float low, float high);

        std::shared_ptr<Scatter> plot_points(MatrixX2f points, Vector4f
            color={0.0, 0.0, 0.0, 1.0}, float size=15.0);
        std::shared_ptr<Line> plot_line(MatrixX2f points, Vector4f color={0.0, 0.0, 0.0, 1.0});

        void display_fps();

      private:
        void draw_pass();

        Window w_;
        std::shared_ptr<ShaderProgram> point_program_;
        std::shared_ptr<ShaderProgram> line_program_;
        Axes axes_;
        std::vector<std::shared_ptr<Scatter>> scatter_plots_;
        std::vector<std::shared_ptr<Line>> line_plots_;
    };

  } // namespace plot
} // namespace cannon

#endif /* ifndef CANNON_PLOT_PLOTTER_H */
