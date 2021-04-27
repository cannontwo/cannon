#ifndef CANNON_PLOT_SCATTER_H
#define CANNON_PLOT_SCATTER_H 

#include <glad/glad.h>
#include <Eigen/Dense>

#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>
#include <cannon/log/registry.hpp>
#include <cannon/plot/plotter.hpp>

using namespace Eigen;
using namespace cannon::graphics;
using namespace cannon::log;

namespace cannon {
  namespace plot {

    class Plotter;

    class Scatter {
      public:
        Scatter() = delete;

        Scatter(Plotter& plotter, std::shared_ptr<ShaderProgram> program,
            MatrixX2f points, Vector4f color, float point_size) :
            plotter_(plotter), points_(points), color_(color),
            point_size_(point_size), vao_(new VertexArrayObject), buf_(vao_),
            program_(program)  {
          glEnable(GL_PROGRAM_POINT_SIZE);
          buf_.buffer(points_);
          log_info(buf_);
        }

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
        std::shared_ptr<VertexArrayObject> vao_;
        VertexBuffer buf_;
        std::shared_ptr<ShaderProgram> program_;
    };

  } // namespace plot
} // namespace cannon

#endif /* ifndef CANNON_PLOT_SCATTER_H */
