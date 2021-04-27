#ifndef CANNON_PLOT_LINE_H
#define CANNON_PLOT_LINE_H 

#include <glad/glad.h>
#include <Eigen/Dense>

#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>
#include <cannon/plot/plotter.hpp>
#include <cannon/log/registry.hpp>

using namespace Eigen;
using namespace cannon::graphics;
using namespace cannon::log;

namespace cannon {
  namespace plot {

    class Plotter;

    class Line {
      public:
        Line() = delete;

        Line(Plotter& plotter, std::shared_ptr<ShaderProgram> program,
            MatrixX2f points, Vector4f color) : plotter_(plotter),
        points_(points), color_(color), vao_(new VertexArrayObject),
        buf_(vao_), program_(program)  {
          buf_.buffer(points_);
        }

        Line(Line& s) = delete;

        Line(Line&& s) : plotter_(s.plotter_), points_(s.points_), color_(s.color_),
          vao_(s.vao_), buf_(std::move(s.buf_)),
          program_(std::move(s.program_)) {}

        void add_points(MatrixX2f point);
        void replace_points(MatrixX2f points);

        friend class Plotter;

      private:
        void draw();
        void update_plotter();

        Plotter& plotter_;
        MatrixX2f points_;
        Vector4f color_;
        std::shared_ptr<VertexArrayObject> vao_;
        VertexBuffer buf_;
        std::shared_ptr<ShaderProgram> program_;
    };

  } // namespace plot
} // namespace cannon

#endif /* ifndef CANNON_PLOT_LINE_H */
