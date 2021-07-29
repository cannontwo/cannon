#ifndef CANNON_PLOT_LINE_H
#define CANNON_PLOT_LINE_H 

/*!
 * \file cannon/plot/line.hpp
 * \brief File containing Line class definition for plotting line plots.
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
     * \brief Class representing a line plot in the context of a Plotter. A
     * line plot is a sequence of points which will be displayed connected by
     * straight line segments.
     */
    class Line {
      public:
        Line() = delete;

        /*!
         * \brief Constructor taking a plotter reference, shader, points, color
         * for the line plot, and line width.
         */
        Line(Plotter &plotter, ShaderProgramPtr program, MatrixX2f points,
            Vector4f color, float line_width=2.0);

        Line(Line& s) = delete;

        /*!
         * \brief Move constructor.
         */
        Line(Line&& s) : plotter_(s.plotter_), points_(s.points_), color_(s.color_),
          vao_(s.vao_), buf_(std::move(s.buf_)),
          program_(std::move(s.program_)) {}

        /*!
         * \brief Append multiple points to this line plot. Each row is
         * interpreted as a point.
         *
         * \param point The points to add.
         */
        void add_points(MatrixX2f point);

        /*!
         * \brief Append a single point to this line plot.
         *
         * \param point The point to add.
         */
        void add_point(Vector2f point);
        
        /*!
         * \brief Replace the points of this line plot by new ones. Each row is
         * interpreted as a point.
         *
         * \param points New points for this plot.
         */
        void replace_points(MatrixX2f points);

        /*!
         * \brief Draw this line plot in the current OpenGL context.
         */
        void draw();

        friend class Plotter;

      private:
        
        /*!
         * \brief Update the plotter that this line plot is defined in
         * reference to. Mostly used to update mins and maxes of the plotter.
         */
        void update_plotter();

        Plotter& plotter_; //!< Plotter that this line plot is associated with
        MatrixX2f points_; //!< Points making up this line plot
        Vector4f color_; //!< Color for this line plot
        VertexArrayObjectPtr vao_; //!< VAO for drawing the line plot
        VertexBufferPtr buf_; //!< Vertex buffer for drawing the line plot
        ShaderProgramPtr program_; //!< Shader for drawing

        float line_width_; //!< Line width for drawing line plot
    };

  } // namespace plot
} // namespace cannon

#endif /* ifndef CANNON_PLOT_LINE_H */
