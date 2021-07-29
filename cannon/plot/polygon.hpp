#ifndef CANNON_PLOT_POLYGON_H
#define CANNON_PLOT_POLYGON_H 

#include <Eigen/Dense>

#include <cannon/utils/class_forward.hpp>

using namespace Eigen;

namespace cannon {

  namespace graphics {
    CANNON_CLASS_FORWARD(ShaderProgram);
    CANNON_CLASS_FORWARD(VertexArrayObject);
    CANNON_CLASS_FORWARD(VertexBuffer);
  }
  
  using namespace cannon::graphics;

  namespace plot {

    class Polygon {
      public:
        Polygon() = delete;

        Polygon(ShaderProgramPtr program, const std::vector<Vector2d>& poly, const
            MatrixX4f& colors);

        Polygon(ShaderProgramPtr program, const std::vector<Vector2d>& poly, const
            Vector4f& color);

        Polygon(Polygon& o) = delete;

        Polygon(Polygon&& o) : points_(o.points_), color_(o.color_),
        vao_(o.vao_), buf_(std::move(o.buf_)), program_(std::move(o.program_)) {}

        void draw();

        friend class Plotter;

      private:
        MatrixX2f points_;
        MatrixX4f color_;
        VertexArrayObjectPtr vao_;
        VertexBufferPtr buf_;
        VertexBufferPtr color_buf_;
        ShaderProgramPtr program_;

    };

  } // namespace plot
} // namespace cannon

#endif /* ifndef CANNON_PLOT_POLYGON_H */
