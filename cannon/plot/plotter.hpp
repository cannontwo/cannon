#ifndef CANNON_PLOT_PLOTTER_H
#define CANNON_PLOT_PLOTTER_H 

#include <string>
#include <vector>
#include <stdexcept>

#include <cannon/graphics/window.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>
#include <cannon/plot/scatter.hpp>
#include <cannon/plot/axes.hpp>
#include <cannon/log/registry.hpp>
#include <cannon/plot/line.hpp>
#include <cannon/plot/polygon.hpp>

using namespace cannon::graphics;
using namespace cannon::log;

namespace cannon {
  namespace plot {

    class Scatter;
    class Line;
    class Polygon;

    class Plotter {
      public:
        Plotter(bool axes_outside=false) : w_(), axes_(1.0, axes_outside),
            axes_outside_(axes_outside), point_program_(new ShaderProgram),
            line_program_(new ShaderProgram), poly_program_(new ShaderProgram)  {

          //glEnable(GL_BLEND);
          //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
          //glDisable(GL_DEPTH_TEST);

          w_.set_clear_color(Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
          w_.disable_depth_test();
          w_.disable_face_culling();
          w_.disable_stencil_test();

          // Set up point shader
          VertexShader v = load_vertex_shader("shaders/basic_2d_pass_pos.vert");
          FragmentShader f = load_fragment_shader("shaders/uniform_color_circle.frag");

          point_program_->attach_shader(v);
          point_program_->attach_shader(f);
          point_program_->link();

          // Set up line shader
          const char *fl_src = BASIC_FRAGMENT_SHADER.c_str();
          VertexShader vl = load_vertex_shader("shaders/basic_2d_pass_pos.vert");
          FragmentShader fl = FragmentShader(&fl_src);

          line_program_->attach_shader(vl);
          line_program_->attach_shader(fl);
          line_program_->link();


          // TODO Make Polygon vertex shader with per-vertex colors
          VertexShader poly_vl = load_vertex_shader("shaders/2d_pass_pos_color.vert");
          FragmentShader poly_fl = load_fragment_shader("shaders/pass_color_viridis.frag");
          poly_program_->attach_shader(poly_vl);
          poly_program_->attach_shader(poly_fl);
          poly_program_->link();
        }

        void render();
        void close() const;

        ImageData get_image() const;
        void save_image(const std::string& path);

        // Variant for rendering an animated plot
        template <typename F>
        void render(F f) {
          w_.render_loop([this, f] {draw_pass();f();});
        }

        void set_xlim(float low, float high);
        void set_ylim(float low, float high);

        std::shared_ptr<Scatter> plot_points(MatrixX2f points, Vector4f
            color={0.0, 0.0, 0.0, 1.0}, float size=15.0);
        std::shared_ptr<Line> plot_line(MatrixX2f points, Vector4f color={0.0, 0.0, 0.0, 1.0});

        // TODO We need two kinds of polygon plotting: contour plots that
        // should use the viridis shader and polygons that support arbitrary
        // colors
        std::shared_ptr<Polygon> plot_polygon(const Polygon_2& poly, const MatrixX4f& color);
        std::shared_ptr<Polygon> plot_polygon(const Polygon_2& poly, const Vector4f& color);

        void display_fps();

        void write_imgui();

        Window w_;
        Axes axes_;

      private:
        void draw_pass();

        bool axes_outside_;

        std::shared_ptr<ShaderProgram> point_program_;
        std::shared_ptr<ShaderProgram> line_program_;
        std::shared_ptr<ShaderProgram> poly_program_;

        std::vector<std::shared_ptr<Scatter>> scatter_plots_;
        std::vector<std::shared_ptr<Line>> line_plots_;
        std::vector<std::shared_ptr<Polygon>> polygon_plots_;

        std::thread render_thread_;

        // TODO Make optional parameter for Plotter, remove individual scatter
        // point size setting. Alternatively, do differently-colored point
        // scatter plotting better.
        float scatter_point_size_ = 15.0;
    };

  } // namespace plot
} // namespace cannon

#endif /* ifndef CANNON_PLOT_PLOTTER_H */
