#ifndef CANNON_PLOT_LINE_H
#define CANNON_PLOT_LINE_H 

#include <glad/glad.h>
#include <Eigen/Dense>

#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/utils/class_forward.hpp>

using namespace Eigen;
using namespace cannon::graphics;

namespace cannon {

  namespace graphics {
    CANNON_CLASS_FORWARD(ShaderProgram);
  }

  namespace plot {

    CANNON_CLASS_FORWARD(Plotter);

    class Line {
      public:
        Line() = delete;

        Line(Plotter &plotter, ShaderProgramPtr program, MatrixX2f points,
            Vector4f color, float line_width=2.0);

        Line(Line& s) = delete;

        Line(Line&& s) : plotter_(s.plotter_), points_(s.points_), color_(s.color_),
          vao_(s.vao_), buf_(std::move(s.buf_)),
          program_(std::move(s.program_)) {}

        void add_points(MatrixX2f point);
        void add_point(Vector2f point);
        void replace_points(MatrixX2f points);

        friend class Plotter;

      private:
        void draw();
        void update_plotter();

        Plotter& plotter_;
        MatrixX2f points_;
        Vector4f color_;
        VertexArrayObjectPtr vao_;
        VertexBuffer buf_;
        ShaderProgramPtr program_;

        float line_width_;
    };

  } // namespace plot
} // namespace cannon

#endif /* ifndef CANNON_PLOT_LINE_H */
