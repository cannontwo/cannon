#include <cannon/plot/polygon.hpp>

using namespace cannon::plot;

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
