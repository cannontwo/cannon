#ifndef CANNON_PLOT_AXES_H
#define CANNON_PLOT_AXES_H 

/*!
 * \file cannon/plot/axes.hpp
 * \brief File containing Axes class definition.
 */

/*!
 * \namespace cannon::plot
 * \brief Namespace containing plotting utilities for generating
 * two-dimensional scientific charts.
 */

#include <string>
#include <Eigen/Dense>

#include <cannon/utils/class_forward.hpp>

using namespace Eigen;

namespace cannon {
  
  namespace graphics {
    CANNON_CLASS_FORWARD(VertexArrayObject);
    CANNON_CLASS_FORWARD(ShaderProgram);
    CANNON_CLASS_FORWARD(VertexBuffer);
    CANNON_CLASS_FORWARD(Font);
  }

  using namespace cannon::graphics;

  namespace plot {

    /*!
     * \brief Class representing the 2D axes on which a particular plot is
     * plotted. This class is responsible for handling the screen-space scaling
     * and display of axis bars for plots.
     */
    class Axes {
      public:
        /*!
         * \brief Constructor taking a text scale for the axis tick labels and
         * whether axis should be displayed on the outside of the plot.
         */
        Axes(float text_scale=1.0, bool outside=false);

        /*!
         * \brief Draw the x- and y-axis represented by this Axes object. This
         * method assumes that OpenGL has already been initialized and a window
         * created.
         */
        void draw();

        /*!
         * \brief Update scaling and ticks using stored axes extents.
         */
        void update_limits();

        /*!
         * \brief Update the limits of the axes managed by this object and
         * mapping to screen space.
         *
         * \param x_min New minimum x-value
         * \param x_max New maximum x-value
         * \param y_min New minimum y-value
         * \param y_max New maximum y-value
         * \param window_width New window width
         * \param window_height New window height
         */
        void update_limits(float x_min, float x_max, float y_min, float y_max,
            int window_width, int window_height);

        /*!
         * \brief Update axis tick text scale using window dimensions.
         *
         * \param width New window width
         * \param height New window height
         */
        void update_scale(int width, int height);

        /*!
         * \brief Set x-axis tick marks. Ticks will no longer be automatically placed.
         *
         * \param ticks X values for tick locations, in plot space.
         */
        void set_xticks(std::vector<float> ticks);

        /*!
         * \brief Set y-axis tick marks. Ticks will no longer be automatically placed.
         *
         * \param ticks Y values for tick locations, in plot space.
         */
        void set_yticks(std::vector<float> ticks);

        /*!
         * \brief Write ImGui controls for this object.
         */
        void write_imgui();

        /*!
         * \brief Create transformation matrix mapping plot space to screen
         * space, using stored axes information.
         *
         * \returns Scaling matrix for current axes information.
         */
        Matrix4f make_scaling_matrix();
        // TODO Make separate scaling matrix for drawing actual axes, in order
        // to prevent overlapped plotting

      private:

        /*!
         * \brief Update positions of x and y axis tick marks using axis
         * extents.
         */
        void update_ticks();

        /*!
         * \brief Compute scaled padding around plot with respect to total
         * extent of axes.
         *
         * \returns Vector containing the x padding and y padding.
         */
        Vector2f get_scaled_padding();

        /*!
         * \brief Compute the padded size of the overall plot in plot space. 
         *
         * \returns A matrix consisting of the padded x minimum and maximum on
         * the first row, then the padded y minimum and maximum on the second
         * row.
         */
        Matrix2f get_padded_extent();
        
        /*!
         * \brief Get the string for an axis tick at the input location. 
         *
         * \param tick Tick location on an axis.
         *
         * \returns String representation of the tick location.
         */
        std::string get_tick_string(float tick);

        float x_min_; //!< Minimum plotted x-value
        float x_max_; //!< Maximum plotted x-value
        float y_min_; //!< Minimum plotted y-value
        float y_max_; //!< Maximum plotted y-value

        float text_offset_x_ = 0.0; //!< Default offset of x ticks from x axis
        float text_offset_y_ = 0.0; //!< Default offset of y ticks from y axis
        float axis_line_width_ = 1; //!< Default axis line width

        float text_scale_x_; //!< Text x scale for tick display
        float text_scale_y_; //!< Text y scale for tick display

        bool outside_; //!< Whether to plot axis lines "outside" of the plotted area
        float padding_; //!< Padding amount as a proportion of plotted extent
        float text_scale_multiplier_; //!< Text scaling parameter

        bool auto_ticks_ = true; //!< Whether to automatically select tick locations
        std::vector<float> x_ticks_; //!< X axis tick locations
        std::vector<float> y_ticks_; //!< Y axis tick locations

        ShaderProgramPtr program_; //!< Shader used for axis drawing
        VertexArrayObjectPtr vao_; //!< VAO used for axis drawing
        VertexBufferPtr buf_; //!< Buffer used for axis drawing

        FontPtr font_; //!< Font for tick labels
        ShaderProgramPtr text_program_; //!< Shader for drawing tick labels
        VertexArrayObjectPtr text_vao_; //!< VAO for drawing tick labels
        VertexBufferPtr text_quad_buf_; //!< Buffer used for drawing tick labels
    };

  } // namespace plot
} // namespace cannon

#endif /* ifndef CANNON_PLOT_AXES_H */
