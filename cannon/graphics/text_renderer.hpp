#ifndef CANNON_GRAPHICS_TEXT_RENDERER_H
#define CANNON_GRAPHICS_TEXT_RENDERER_H

/*!
 * \file cannon/graphics/text_renderer.hpp
 * \brief File containing TextRenderer class definition.
 */

#include <Eigen/Dense>

#include <cannon/utils/class_forward.hpp>

using namespace Eigen;

namespace cannon {
  namespace graphics {

    CANNON_CLASS_FORWARD(Font);
    CANNON_CLASS_FORWARD(VertexArrayObject);
    CANNON_CLASS_FORWARD(VertexBuffer);
    CANNON_CLASS_FORWARD(ShaderProgram);

    /*!
     * \brief Struct representing a bounding box around a line of text.
     */
    struct TextBoundingBox {
      double x; //!< X-coordinate of left edge of bounding box
      double y; //!< Y-coordinate of lower edge of bounding box
      double width; //!< Width of bounding box
      double height; //!< Height of bounding box
    };

    /*!
     * \brief Class handling the rendering of strings in a font to specific
     * regions of screen-space.
     */
    class TextRenderer {
      public:

        /*!
         * \brief Constructor taking the font to render text in.
         */
        TextRenderer(unsigned int width, unsigned int height, FontPtr font);

        /*!
         * \brief Draw the input string at the input position and scale in
         * screen space.
         *
         * \param text The text to draw
         * \param x X-coordinate of lower-left corner of text
         * \param y Y-coordinate of lower-left corner of text
         * \param scale Text rendering scale
         */
        void draw(const std::string &text, double x, double y, double scale = 1.0);

        /*!
         * \brief Draw the input string within the input bounding box.
         *
         * \param text The string to draw
         * \param box The box to draw text within.
         */
        void draw(const std::string &text, const TextBoundingBox& box);

        /*!
         * \brief Compute bounding box for the input text to be rendered. Does
         * not draw to the screen.
         *
         * \param text The text to render
         * \param x X-coordinate of start for text rendering
         * \param y Y-coordinate of start for text rendering
         * \param scale Scale to compute bounding box for.
         *
         * \returns Parameters of the bounding box around the input text.
         */
        TextBoundingBox compute_bounding_box(const std::string &text, double x,
                                              double y, double scale = 1.0);
        /*!
         * \brief Get a mutable reference to the text color used by this
         * renderer.
         *
         * \returns Reference to text color.
         */
        Vector4f& text_color();

        /*!
         * \brief Get a mutable reference to the screen width used by this
         * renderer.
         *
         * \returns Reference to width.
         */
        unsigned int& width();

        /*!
         * \brief Get a mutable reference to the screen height used by this
         * renderer.
         *
         * \returns Reference to height.
         */
        unsigned int& height();

      private:
        unsigned int width_; //!< Screen space width
        unsigned int height_; //!< Screen space height

        FontPtr font_; //!< Font for text rendering
        VertexArrayObjectPtr vao_; //!< Vertex array object for text rendering
        VertexBufferPtr buf_; //!< Buffer for drawing text
        ShaderProgramPtr program_; //!< Shader for drawing text

        Vector4f text_color_; //!< Text color
    
    };

    /*!
     * \brief Compute the scaling factor that should be applied to the source
     * bounding box so that the scaled bounding box will fit snugly inside the
     * target bounding box.
     *
     * \param source Bounding box to be rescaled
     * \param target Bounding box serving as scale target
     *
     * \returns Scaling necessary
     */
    double get_box_scale(const TextBoundingBox& source, const TextBoundingBox& target);

  }
} 

#endif /* ifndef CANNON_GRAPHICS_TEXT_RENDERER_H */
