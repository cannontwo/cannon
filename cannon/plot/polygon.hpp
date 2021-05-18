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

#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/utils/class_forward.hpp>

using namespace Eigen;

using namespace cannon::graphics;

namespace cannon {

  namespace graphics {
    CANNON_CLASS_FORWARD(ShaderProgram);
    CANNON_CLASS_FORWARD(VertexArrayObject);
  }

  namespace plot {

    class Polygon {
      public:
        Polygon() = delete;

        Polygon(ShaderProgramPtr program, const Polygon_2& cgal_poly, const
            MatrixX4f& colors);

        Polygon(ShaderProgramPtr program, const Polygon_2& cgal_poly, const
            Vector4f& color);

        Polygon(Polygon& o) = delete;

        Polygon(Polygon&& o) : points_(o.points_), color_(o.color_),
        vao_(o.vao_), buf_(std::move(o.buf_)), program_(std::move(o.program_)) {}

        friend class Plotter;

      private:
        void draw();

        std::vector<Polygon_2> extract_triangles_(const Polygon_2& cgal_poly);

        MatrixX2f points_;
        MatrixX4f color_;
        VertexArrayObjectPtr vao_;
        VertexBuffer buf_;
        VertexBuffer color_buf_;
        ShaderProgramPtr program_;

    };

  } // namespace plot
} // namespace cannon

#endif /* ifndef CANNON_PLOT_POLYGON_H */
