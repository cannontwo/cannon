#include <cannon/plot/polygon.hpp>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>

using K = CGAL::Exact_predicates_exact_constructions_kernel;
using Polygon_2 = CGAL::Polygon_2<K>;

#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/geom/kd_tree_indexed.hpp>
#include <cannon/geom/extract_triangles.hpp>

using namespace cannon::plot;
using namespace cannon::geom;

Polygon::Polygon(ShaderProgramPtr program, const std::vector<Vector2d> &poly,
                 const MatrixX4f &colors)
    : vao_(new VertexArrayObject), buf_(new VertexBuffer(vao_)),
      color_buf_(new VertexBuffer(vao_)), program_(program) {

  KDTreeIndexed tree(2);
  Polygon_2 cgal_poly;
  for (const auto &v : poly) {
    K::Point_2 p(v[0], v[1]);
    cgal_poly.push_back(p);
    tree.insert(v);
  }

  auto triangles = extract_triangles(cgal_poly);
  points_ = MatrixX2f::Zero(3 * triangles.size(), 2);
  color_ = MatrixX4f::Zero(3 * triangles.size(), 4);

  for (unsigned int i = 0; i < triangles.size(); i++) {
    assert(triangles[i].size() == 3);
    for (unsigned int j = 0; j < 3; j++) {
      points_(3*i + j, 0) = CGAL::to_double(triangles[i].vertex(j).x());
      points_(3*i + j, 1) = CGAL::to_double(triangles[i].vertex(j).y());

      int color_idx = tree.get_nearest_idx(points_.row(3*i + j).transpose().cast<double>());
      color_.row(3*i + j) = colors.row(color_idx);
    }
  }

  buf_->buffer(points_);
  color_buf_->buffer(color_);
}

Polygon::Polygon(ShaderProgramPtr program, const std::vector<Vector2d> &poly,
                 const Vector4f &color)
    : vao_(new VertexArrayObject), buf_(new VertexBuffer(vao_)),
      color_buf_(new VertexBuffer(vao_)), program_(program) {

  Polygon_2 cgal_poly;
  for (auto &v : poly) {
    K::Point_2 p(v[0], v[1]);
    cgal_poly.push_back(p);
  }

  auto triangles = extract_triangles(cgal_poly);
  points_ = MatrixX2f::Zero(3 * triangles.size(), 2);
  color_ = MatrixX4f::Zero(3 * triangles.size(), 4);

  for (unsigned int i = 0; i < triangles.size(); i++) {
    assert(triangles[i].size() == 3);
    for (unsigned int j = 0; j < 3; j++) {
      points_(3*i + j, 0) = CGAL::to_double(triangles[i].vertex(j).x());
      points_(3*i + j, 1) = CGAL::to_double(triangles[i].vertex(j).y());

      color_.row(3*i + j) = color;
    }
  }

  buf_->buffer(points_);
  color_buf_->buffer(color_);
}

void Polygon::draw() {
  program_->activate();

  buf_->bind();
  color_buf_->bind();
  glDrawArrays(GL_TRIANGLES, 0, points_.rows());
}
