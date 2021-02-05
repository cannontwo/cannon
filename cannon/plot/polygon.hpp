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

using namespace Eigen;


namespace cannon {
  namespace plot {

    class Plotter;


    class Polygon {
      public:
        Polygon() = delete;

        Polygon(Plotter& plotter, std::shared_ptr<ShaderProgram> program, const
            Polygon_2& cgal_poly, Vector4f color) : plotter_(plotter),
        color_(color), vao_(new VertexArrayObject), buf_(vao_),
        program_(program) {

          auto triangles = extract_triangles_(cgal_poly);
          points_ = MatrixX2f::Zero(3 * triangles.size(), 2);

          for (unsigned int i = 0; i < triangles.size(); i++) {
            assert(triangles[i].size() == 3);
            for (unsigned int j = 0; j < 3; j++) {
              points_(3*i + j, 0) = CGAL::to_double(triangles[i].vertex(j).x());
              points_(3*i + j, 1) = CGAL::to_double(triangles[i].vertex(j).y());
            }
          }

          buf_.buffer(points_);
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
        Vector4f color_;
        std::shared_ptr<VertexArrayObject> vao_;
        VertexBuffer buf_;
        std::shared_ptr<ShaderProgram> program_;

    };

  } // namespace plot
} // namespace cannon

#endif /* ifndef CANNON_PLOT_POLYGON_H */
