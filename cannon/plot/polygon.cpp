#include <cannon/plot/polygon.hpp>

#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/geom/kd_tree_indexed.hpp>

using namespace cannon::plot;
using namespace cannon::geom;

Polygon::Polygon(ShaderProgramPtr program, const Polygon_2& cgal_poly, const
    MatrixX4f& colors) : vao_(new VertexArrayObject), buf_(vao_),
  color_buf_(vao_), program_(program) {

  KDTreeIndexed tree(2);
  for (auto it = cgal_poly.vertices_begin(); it < cgal_poly.vertices_end(); it++) {
    Vector2d vert = Vector2d::Zero();
    vert[0] = CGAL::to_double(it->x());
    vert[1] = CGAL::to_double(it->y());

    tree.insert(vert);
  }

  auto triangles = extract_triangles_(cgal_poly);
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

  buf_.buffer(points_);
  color_buf_.buffer(color_);
}

Polygon::Polygon(ShaderProgramPtr program, const Polygon_2& cgal_poly, const
    Vector4f& color) : vao_(new VertexArrayObject), buf_(vao_),
  color_buf_(vao_), program_(program) {

  auto triangles = extract_triangles_(cgal_poly);
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

  buf_.buffer(points_);
  color_buf_.buffer(color_);
}

void Polygon::draw() {
  program_->activate();

  buf_.bind();
  color_buf_.bind();
  glDrawArrays(GL_TRIANGLES, 0, points_.rows());
}

std::vector<Polygon_2> Polygon::extract_triangles_(const Polygon_2& cgal_poly) { 
  std::vector<Polygon_2> ret_vec;

  CDT cdt;
  cdt.insert_constraint(cgal_poly.vertices_begin(), cgal_poly.vertices_end(), true);

  for (auto& fh : cdt.finite_face_handles()) {
    Polygon_2 p;
    p.push_back(fh->vertex(0)->point());
    p.push_back(fh->vertex(1)->point());
    p.push_back(fh->vertex(2)->point());

    assert(p.orientation() == CGAL::COUNTERCLOCKWISE);
    ret_vec.push_back(p);
  }
   
  return ret_vec;
}
