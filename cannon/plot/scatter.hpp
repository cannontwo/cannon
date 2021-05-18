#ifndef CANNON_PLOT_SCATTER_H
#define CANNON_PLOT_SCATTER_H 

#include <glad/glad.h>
#include <Eigen/Dense>

#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/utils/class_forward.hpp>

using namespace Eigen;

using namespace cannon::graphics;

namespace cannon {

  namespace graphics {
    CANNON_CLASS_FORWARD(ShaderProgram);
    CANNON_CLASS_FORWARD(VertexArrayObject);
  }

  namespace plot {

    CANNON_CLASS_FORWARD(Plotter);

    class Scatter {
      public:
        Scatter() = delete;

        Scatter(Plotter& plotter, ShaderProgramPtr program, MatrixX2f points,
            Vector4f color, float point_size);

        Scatter(Scatter& s) = delete;

        Scatter(Scatter&& s) : plotter_(s.plotter_), points_(s.points_), color_(s.color_),
          point_size_(s.point_size_), vao_(s.vao_), buf_(std::move(s.buf_)),
          program_(std::move(s.program_)) {}

        void add_points(MatrixX2f point);

        friend class Plotter;

      private:
        void draw();

        Plotter& plotter_;
        MatrixX2f points_;
        Vector4f color_;
        float point_size_;
        VertexArrayObjectPtr vao_;
        VertexBuffer buf_;
        ShaderProgramPtr program_;
    };

  } // namespace plot
} // namespace cannon

#endif /* ifndef CANNON_PLOT_SCATTER_H */
