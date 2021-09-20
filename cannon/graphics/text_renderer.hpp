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
         */
        void draw(const std::string &text, double x, double y, double scale = 1.0);

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

  }
} 

#endif /* ifndef CANNON_GRAPHICS_TEXT_RENDERER_H */
