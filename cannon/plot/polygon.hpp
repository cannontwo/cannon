#ifndef CANNON_PLOT_POLYGON_H
#define CANNON_PLOT_POLYGON_H 

/*!
 * \file cannon/plot/polygon.hpp
 * \brief File containing Polygon class definition for plotting.
 */

#include <Eigen/Dense>

#include <cannon/utils/class_forward.hpp>

using namespace Eigen;

namespace cannon {

  namespace graphics {
    CANNON_CLASS_FORWARD(ShaderProgram);
    CANNON_CLASS_FORWARD(VertexArrayObject);
    CANNON_CLASS_FORWARD(VertexBuffer);
  }
  
  using namespace cannon::graphics;

  namespace plot {

    /*!
     * \brief Class representing a polygon to be plotted by a Plotter. Used for
     * general surface plotting.
     */
    class Polygon {
      public:
        Polygon() = delete;

        /*!
         * \brief Constructor taking a shader for drawing, a vector of vertices
         * for the polygon, and a matrix of colors for each vertex. Each row in
         * the matrix is interpreted as a separate vertex color.
         */
        Polygon(ShaderProgramPtr program, const std::vector<Vector2d>& poly, const
            MatrixX4f& colors);

        /*!
         * \brief Constructor taking a shader for drawing, a vector of vertices
         * for the polygon, and a color.
         */
        Polygon(ShaderProgramPtr program, const std::vector<Vector2d>& poly, const
            Vector4f& color);

        Polygon(Polygon& o) = delete;

        /*!
         * \brief Move constructor.
         */
        Polygon(Polygon&& o) : points_(o.points_), color_(o.color_),
        vao_(o.vao_), buf_(std::move(o.buf_)), program_(std::move(o.program_)) {}

        /*!
         * \brief Draw this polygon.
         */
        void draw();

        friend class Plotter;

      private:
        MatrixX2f points_; //!< Points defining the vertices of this polygon
        MatrixX4f color_; //!< Colors to associate with each vertex of this polygon

        VertexArrayObjectPtr vao_; //!< VAO for drawing this polygon
        VertexBufferPtr buf_; //!< VertexBuffer for drawing this polygon
        VertexBufferPtr color_buf_; //!< Buffer for storing colors for drawing
        ShaderProgramPtr program_; //!< Shader used to draw this polygon

    };

  } // namespace plot
} // namespace cannon

#endif /* ifndef CANNON_PLOT_POLYGON_H */
