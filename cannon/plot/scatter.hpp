#ifndef CANNON_PLOT_SCATTER_H
#define CANNON_PLOT_SCATTER_H 

/*!
 * \file cannon/plot/scatter.hpp
 * \brief File containing Scatter class definition.
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

    CANNON_CLASS_FORWARD(Plotter);

    /*!
     * \brief Class representing a scatter plot which can be drawn by a
     * Plotter.
     */
    class Scatter {
      public:
        Scatter() = delete;

        /*!
         * \brief Constructor taking a plotter, shader program for drawing,
         * points in the scatter plot, and color for the points, as well as a
         * size for the displayed points.
         */
        Scatter(Plotter& plotter, ShaderProgramPtr program, MatrixX2f points,
            Vector4f color, float point_size);

        Scatter(Scatter& s) = delete;

        /*!
         * \brief Move constructor.
         */
        Scatter(Scatter&& s) : plotter_(s.plotter_), points_(s.points_), color_(s.color_),
          point_size_(s.point_size_), vao_(s.vao_), buf_(std::move(s.buf_)),
          program_(std::move(s.program_)) {}

        /*!
         * \brief Add points to this scatter plot.
         *
         * \param point The points to add. Each row in the matrix is a separate
         * point.
         */
        void add_points(MatrixX2f point);

        /*!
         * \brief Draw this scatter plot.
         */
        void draw();

        friend class Plotter;

      private:
        Plotter& plotter_; //!< Plotter that this scatter plot is a part of
        MatrixX2f points_; //!< Points to be drawn as part of this scatter plot
        Vector4f color_; //!< Color to draw points
        float point_size_; //!< Point size for displayed points

        VertexArrayObjectPtr vao_; //!< VAO for drawing this scatter plot
        VertexBufferPtr buf_; //!< Buffer for points to be drawn
        ShaderProgramPtr program_; //!< Shader program for drawing scatter plot
    };

  } // namespace plot
} // namespace cannon

#endif /* ifndef CANNON_PLOT_SCATTER_H */
