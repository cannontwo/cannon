#ifndef CANNON_PLOT_PLOTTER_H
#define CANNON_PLOT_PLOTTER_H 

/*!
 * \file cannon/plot/plotter.hpp
 * \brief File containing Plotter class definition.
 */

#include <string>
#include <vector>
#include <functional>

#include <Eigen/Dense>

#include <cannon/utils/class_forward.hpp>

using namespace Eigen;

namespace cannon {
  namespace graphics {
    CANNON_CLASS_FORWARD(ShaderProgram);
    CANNON_CLASS_FORWARD(Window);
    CANNON_CLASS_FORWARD(ImageData);
  }

  using namespace cannon::graphics;

  namespace plot {

    CANNON_CLASS_FORWARD(Axes);
    CANNON_CLASS_FORWARD(Scatter);
    CANNON_CLASS_FORWARD(Line);
    CANNON_CLASS_FORWARD(Polygon);

    /*!
     * \brief Class representing a 2D plotter which can be used to make
     * scientific charts. Can also be used for OpenGL rendering.
     */
    class Plotter {
      public:

        /*!
         * \brief Constructor taking whether plot axes should be drawn outside
         * the plot area.
         */
        Plotter(bool axes_outside=false);

        /*!
         * \brief Render this plotter to the screen.
         */
        void render();

        /*!
         * \brief Render this plotter the the screen, calling the input
         * function on each frame.
         *
         * \param f The update function to call on each frame.
         */
        void render(std::function<void()> f);
        
        /*!
         * \brief Close the rendering window for this plotter.
         */
        void close() const;

        /*!
         * \brief Get image data representing a screenshot of the currently
         * displayed plotter.
         *
         * \returns ImageData containing the screenshot.
         */
        ImageData get_image() const;
        
        /*!
         * \brief Save the currently rendered plot to the input path.
         *
         * \param path The path to save the rendered image to.
         */
        void save_image(const std::string& path);

        /*!
         * \brief Clear the current plot data.
         */
        void clear();

        /*!
         * \brief Set limits of x-axis for this plot.
         *
         * \param low New lower limit of the x-axis.
         * \param hight New upper limit of the x-axis.
         */
        void set_xlim(float low, float high);

        /*!
         * \brief Set limits of y-axis for this plot.
         *
         * \param low New lower limit of the y-axis.
         * \param hight New upper limit of the y-axis.
         */
        void set_ylim(float low, float high);

        /*!
         * \brief Plot the input points as a scatter plot.
         *
         * \param points Matrix of points to plot, where each row is a separate
         * point.
         * \param color The color of the scatter plot.
         * \param size Point size for the scatter plot.
         *
         * \returns A pointer to the generated scatter plot object.
         */
        ScatterPtr plot_points(MatrixX2f points, Vector4f color={0.0, 0.0, 0.0,
            1.0}, float size=15.0);
        
        /*!
         * \brief Plot the input points as a line plot. 
         *
         * \param points Matrix of points to plot, where each row is a separate
         * point.
         * \param color Color of the line plot.
         *
         * \returns A pointer to the generated line plot object.
         */
        LinePtr plot_line(MatrixX2f points, Vector4f color={0.0, 0.0, 0.0, 1.0});

        // TODO We need two kinds of polygon plotting: contour plots that
        // should use the viridis shader and polygons that support arbitrary
        // colors
        //
        /*!
         * \brief Plot a polygon with interpolated colors.
         *
         * \param poly A vector of vertices making up the polygon, in
         * counter-clockwise order.
         * \param color A matrix of colors for the vertices, where each row is
         * a separate color.
         *
         * \returns A pointer to the generated polygon plot object.
         */
        PolygonPtr plot_polygon(const std::vector<Vector2d>& poly, const MatrixX4f& color);

        /*!
         * \brief Plot a polygon.
         *
         * \param poly A vector of vertices making up the polygon, in
         * counter-clockwise order.
         * \param color Vector color to plot the polygon with.
         *
         * \returns A pointer to the generated polygon plot object.
         */
        PolygonPtr plot_polygon(const std::vector<Vector2d>& poly, const Vector4f& color);

        /*!
         * \brief Plotting helper function which automatically samples the
         * input real function.
         *
         * \param f The function to plot
         * \param samples The number of samples from the function to plot
         * \param low Lower limit of input to plot
         * \param high Upper limit of input to plot
         *
         * \returns A pointer to the generated line plot.
         */
        LinePtr plot(std::function<double(double)> f,
                     unsigned int samples = 100, double low = -1.0,
                     double high = 1.0);
        
        /*!
         * \brief Plotting helper function which plots a series of points, as a
         * line plot, assigning each y-value an increasing integer on the
         * x-axis.
         *
         * \param points The v-values to plot.
         *
         * \returns A pointer to the generated line plot.
         */
        LinePtr plot(std::vector<double> points);

        /*!
         * \brief Plotting helper function which plots a series of x- and
         * y-value pairs as a line plot.
         *
         * \param points The points to plot.
         *
         * \returns A pointer to the generated line plot.
         */
        LinePtr plot(std::vector<Vector2d> points);

        /*!
         * \brief Plotting helper function which plots a surface in 3D.
         *
         * \param f The function to plot.
         * \param lattice_dim Resolution of grid to sample function values.
         * \param x_low Lower limit of x-values
         * \param x_high Upper limit of x-values
         * \param y_low Lower limit of y-values
         * \param y_high Upper limit of y-values
         */
        void plot(std::function<double(const Vector2d &)> f,
                  unsigned int lattice_dim = 10, double x_low = -1.0,
                  double x_high = 1.0, double y_low = -1.0,
                  double y_high = 1.0);

        /*!
         * \brief Display FPS in rendered window.
         */
        void display_fps();

        /*!
         * \brief Write ImGui controls.
         */
        void write_imgui();

        WindowPtr w_; //!< A pointer to the OpenGL window used for plotting.
        AxesPtr axes_; //!< A pointer to the axes object which manages plot transforms

      private:

        /*!
         * \brief Execute the draw pass for this plotter.
         */
        void draw_pass();

        bool axes_outside_; //!< Whether axes should be drawn outside the plot area

        ShaderProgramPtr point_program_; //!< Shader used for drawing scatter plots
        ShaderProgramPtr line_program_; //!< Shader used for drawing line plots
        ShaderProgramPtr poly_program_; //!< Shader used for drawing polygons

        std::vector<ScatterPtr> scatter_plots_; //!< Scatter plots to be drawn
        std::vector<LinePtr> line_plots_; //!< Line plots to be drawn
        std::vector<PolygonPtr> polygon_plots_; //!< Polygon plots to be drawn

        static std::vector<Vector4f> LINE_COLORS; //!< Automatic line plot colors for convenience

        // TODO Make optional parameter for Plotter, remove individual scatter
        // point size setting. Alternatively, do differently-colored point
        // scatter plotting better.
        float scatter_point_size_ = 15.0; //!< Point size for scatter plots
        float line_plot_size_ = 2.0; //!< Line width for line plots
    };

  } // namespace plot
} // namespace cannon

#endif /* ifndef CANNON_PLOT_PLOTTER_H */
