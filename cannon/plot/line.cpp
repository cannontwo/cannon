#include <cannon/plot/line.hpp>

#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>
#include <cannon/plot/plotter.hpp>

using namespace cannon::plot;

Line::Line(Plotter &plotter, ShaderProgramPtr program, MatrixX2f points,
    Vector4f color) : plotter_(plotter), points_(points), color_(color),
  vao_(new VertexArrayObject), buf_(vao_), program_(program) {

  buf_.buffer(points_);

}

void Line::add_points(MatrixX2f point) {
  points_.conservativeResize(points_.rows() + 1, NoChange);
  points_.row(points_.rows() - 1) = point;

  buf_.buffer(points_);
  update_plotter();
}

void Line::replace_points(MatrixX2f points) {
  points_ = points;
  buf_.replace(points_);
  update_plotter();
}

void Line::draw() {
  program_->set_uniform("uColor", color_);
  program_->activate();

  buf_.bind();
  glLineWidth(2.0);
  glDrawArrays(GL_LINE_STRIP, 0, points_.rows());
  glLineWidth(1.0);
}

void Line::update_plotter() {
  RowVector2f maxes = points_.colwise().maxCoeff();
  RowVector2f mins = points_.colwise().minCoeff();
  plotter_.set_xlim(mins[0], maxes[0]);
  plotter_.set_ylim(mins[1], maxes[1]);
}
