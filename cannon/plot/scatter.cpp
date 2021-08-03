#include <cannon/plot/scatter.hpp>

#include <cannon/log/registry.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/plot/plotter.hpp>
#include <cannon/graphics/vertex_array_object.hpp>

using namespace cannon::plot;
using namespace cannon::log;

Scatter::Scatter(Plotter &plotter, ShaderProgramPtr program, MatrixX2f points,
                 Vector4f color, float point_size)
    : plotter_(plotter), points_(points), color_(color),
      point_size_(point_size), vao_(new VertexArrayObject),
      buf_(new VertexBuffer(vao_)), program_(program) {
  glEnable(GL_PROGRAM_POINT_SIZE);
  buf_->buffer(points_);
  log_info(buf_);
}

void Scatter::add_points(MatrixX2f point) {
  points_.conservativeResize(points_.rows() + 1, NoChange);
  points_.row(points_.rows() - 1) = point;

  buf_->buffer(points_);
  RowVector2f maxes = points_.colwise().maxCoeff();
  RowVector2f mins = points_.colwise().minCoeff();
  plotter_.set_xlim(mins[0], maxes[0]);
  plotter_.set_ylim(mins[1], maxes[1]);
}

void Scatter::draw() {
  program_->set_uniform("uColor", color_);
  program_->set_uniform("pointSize", point_size_);
  program_->activate();

  buf_->bind();
  glDrawArrays(GL_POINTS, 0, points_.rows());
}

