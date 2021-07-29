#include <cannon/geom/extract_triangles.hpp>

#include <CGAL/Constrained_Delaunay_triangulation_2.h>

using Itag = CGAL::Exact_predicates_tag;
using CDT = CGAL::Constrained_Delaunay_triangulation_2<K, CGAL::Default, Itag>;

std::vector<Polygon_2> cannon::geom::extract_triangles(const Polygon_2& cgal_poly) {
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
