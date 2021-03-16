#ifndef CANNON_PLOT_POLYGON_H
#define CANNON_PLOT_POLYGON_H 

#include <Eigen/Dense>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Polygon_2.h>

using K = CGAL::Exact_predicates_exact_constructions_kernel;
using Polygon_2 = CGAL::Polygon_2<K>;

using Itag = CGAL::Exact_predicates_tag;
using CDT = CGAL::Constrained_Delaunay_triangulation_2<K, CGAL::Default, Itag>;

#include <cannon/plot/plotter.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/math/nearly_equal.hpp>
#include <cannon/geom/kd_tree_indexed.hpp>

using namespace Eigen;

using namespace cannon::math;
using namespace cannon::geom;

namespace cannon {
  namespace plot {

    class Plotter;

    class Polygon {
      public:
        Polygon() = delete;

        Polygon(Plotter& plotter, std::shared_ptr<ShaderProgram> program, const
            Polygon_2& cgal_poly, const MatrixX4f& colors) : plotter_(plotter),
        vao_(new VertexArrayObject), buf_(vao_),
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

              // TODO There's definitely a better way to do this, e.g. with a KDT

              //intolor_idx = -1;
              //int poly_idx = 0;
              //for (auto it = cgal_poly.vertices_begin(); it < cgal_poly.vertices_end(); it++) {
              //  double x = CGAL::to_double(it->x());
              //  double y = CGAL::to_double(it->y());
              //  double qx = points_(3*i + j, 0);
              //  double qy = points_(3*i + j, 1);

              //  log_info("Comparing query", points_.row(3*i + j), "to (", x, ",", y, ")");
              //  if (nearly_equal(x, qx) && nearly_equal(y, qy)) {
              //    color_idx = poly_idx;
              //    break;
              //  }

              //  poly_idx++;
              //}

              //if (color_idx == -1) {
              //  throw std::runtime_error("Could not find vertex of polygon corresponding to triangle vertex.");
              //}

              int color_idx = tree.get_nearest_idx(points_.row(3*i + j).transpose().cast<double>());
              color_.row(3*i + j) = colors.row(color_idx);
            }
          }

          buf_.buffer(points_);
          color_buf_.buffer(color_);
        }

        Polygon(Plotter& plotter, std::shared_ptr<ShaderProgram> program, const
            Polygon_2& cgal_poly, const Vector4f& color) : plotter_(plotter),
        vao_(new VertexArrayObject), buf_(vao_),
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

        Polygon(Polygon& o) = delete;

        Polygon(Polygon&& o) : plotter_(o.plotter_), points_(o.points_), color_(o.color_),
        vao_(o.vao_), buf_(std::move(o.buf_)), program_(std::move(o.program_)) {}

        friend class Plotter;

      private:
        void draw();

        std::vector<Polygon_2> extract_triangles_(const Polygon_2& cgal_poly);

        Plotter& plotter_;
        MatrixX2f points_;
        MatrixX4f color_;
        std::shared_ptr<VertexArrayObject> vao_;
        VertexBuffer buf_;
        VertexBuffer color_buf_;
        std::shared_ptr<ShaderProgram> program_;

    };

  } // namespace plot
} // namespace cannon

#endif /* ifndef CANNON_PLOT_POLYGON_H */
