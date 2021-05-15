#pragma once
#ifndef CANNON_GRAPHICS_FONT_H
#define CANNON_GRAPHICS_FONT_H 

/*! 
 * \file cannon/graphics/font.hpp
 * \brief File containing Font class definition.
 */

#include <stdexcept>
#include <string>
#include <map>
#include <memory>

#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <cannon/graphics/character.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::log;

namespace cannon {
  namespace graphics {

    /*!
     * \brief Class representing a font face.
     *
     * This class encapsulates an entire OpenFont font face. It can be used to
     * retrieve the information necessary to draw each glyph in the font.
     * Adapted from https://learnopengl.com/In-Practice/Text-Rendering .
     *
     * \sa cannon::graphics::Character
     */
    class Font {
      public:

        /*!
         * Constructor taking the path of a font file.
         *
         * Does not change OpenGL state.
         *
         * \param do_init Whether to initialize the font in this constructor.
         * \param size Size to render this font.
         * \param path Absolute path to the font to load.
         */
        // Does not change OpenGL state
        Font(bool do_init=true, int size=20, const std::string&
            path="/usr/share/fonts/truetype/open-sans/OpenSans-Regular.ttf") :
            path_(path), size_(size) {
          if (do_init) {
            init();
          }
        }

        Font(Font& f) = delete;
        Font(Font&& f) = delete;

        /*!
         * Destructor. Does not change OpenGL state, but does free FreeType
         * resources.
         */
        ~Font() {
          FT_Done_Face(face_);
          FT_Done_FreeType(ft_);
        }

        /*!
         * Method to initialize this font by acquiring FreeType resources and
         * loading all characters.
         *
         * Does not change OpenGL state
         */
        void init();

        /*!
         * Method to get the representation for a character in this font.
         *
         * \param c The character to retrieve.
         *
         * \returns Character object that can be used for drawing.
         */
        std::shared_ptr<Character> get_char(char c);

      private:

        /*!
         * Method to load character glyphs for this font.
         */
        void load_chars();

        const std::string path_; //!< Path to the OpenType font file this object represents.
        int size_; //!< Font size
        bool initialized_ = false; //!< Whether this font has been initialized

        FT_Library ft_; //!< FreeType library object
        FT_Face face_; //!< FreeType font object
        std::map<char, std::shared_ptr<Character>> chars_; //!< Map storing glyphs for ASCII characters

    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_FONT_H */
