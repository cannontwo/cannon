#ifndef CANNON_PLOT_PLOTTER_H
#define CANNON_PLOT_PLOTTER_H 

#include <string>
#include <vector>
#include <functional>
#include <thread>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <Eigen/Dense>

using K = CGAL::Exact_predicates_exact_constructions_kernel;
using Polygon_2 = CGAL::Polygon_2<K>;

#include <cannon/utils/class_forward.hpp>

using namespace Eigen;

namespace cannon {
  namespace graphics {
    CANNON_CLASS_FORWARD(ShaderProgram);
    CANNON_CLASS_FORWARD(Window);
    CANNON_CLASS_FORWARD(ImageData);
  }

  using namespace cannon::graphics;

  namespace plot {

    CANNON_CLASS_FORWARD(Axes);
    CANNON_CLASS_FORWARD(Scatter);
    CANNON_CLASS_FORWARD(Line);
    CANNON_CLASS_FORWARD(Polygon);

    class Plotter {
      public:

        Plotter(bool axes_outside=false);

        void render();
        void close() const;

        ImageData get_image() const;
        void save_image(const std::string& path);

        // Variant for rendering an animated plot
        void render(std::function<void()> f);

        void clear();

        void set_xlim(float low, float high);
        void set_ylim(float low, float high);

        ScatterPtr plot_points(MatrixX2f points, Vector4f color={0.0, 0.0, 0.0,
            1.0}, float size=15.0);
        LinePtr plot_line(MatrixX2f points, Vector4f color={0.0, 0.0, 0.0, 1.0});

        // TODO We need two kinds of polygon plotting: contour plots that
        // should use the viridis shader and polygons that support arbitrary
        // colors
        PolygonPtr plot_polygon(const Polygon_2& poly, const MatrixX4f& color);
        PolygonPtr plot_polygon(const Polygon_2& poly, const Vector4f& color);

        LinePtr plot(std::function<double(double)> f,
                     unsigned int samples = 100, double low = -1.0,
                     double high = 1.0);

        void plot(std::function<double(const Vector2d &)> f,
                  unsigned int lattice_dim = 10, double low = -1.0,
                  double high = 1.0);

        void display_fps();

        void write_imgui();

        WindowPtr w_;
        AxesPtr axes_;

      private:
        void draw_pass();

        bool axes_outside_;

        ShaderProgramPtr point_program_;
        ShaderProgramPtr line_program_;
        ShaderProgramPtr poly_program_;

        std::vector<ScatterPtr> scatter_plots_;
        std::vector<LinePtr> line_plots_;
        std::vector<PolygonPtr> polygon_plots_;

        std::thread render_thread_;

        // TODO Make optional parameter for Plotter, remove individual scatter
        // point size setting. Alternatively, do differently-colored point
        // scatter plotting better.
        float scatter_point_size_ = 15.0;
    };

  } // namespace plot
} // namespace cannon

#endif /* ifndef CANNON_PLOT_PLOTTER_H */
