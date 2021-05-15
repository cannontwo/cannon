#pragma once
#ifndef CANNON_GRAPHICS_CHARACTER_H
#define CANNON_GRAPHICS_CHARACTER_H

/*!
 * \file cannon/graphics/character.hpp
 * \brief File containing Character class definition.
 */

#include <Eigen/Dense>

#include <cannon/utils/class_forward.hpp>

using namespace Eigen;

namespace cannon {
  namespace graphics {

    CANNON_CLASS_FORWARD(Texture);

    /*!
     * \brief Class representing a character in a font.
     *
     * This class encapsulates the data needed to draw a specific character
     * loaded from an OpenFont font. It is adapted from
     * https://learnopengl.com/In-Practice/Text-Rendering .
     *
     * \sa cannon::graphics::Font
     *
     */
    class Character {
      public:
        Character() = delete;

        /*!
         * Constructor taking the character that this object represents, the
         * texture for the character, and various parameters controlling the
         * character display.
         *
         * \param c The character this object represents in the font.
         * \param t The texture used to draw the character.
         * \param s Size of the character glyph.
         * \param b Bearing (offset) of the character glyph.
         * \param a Advance from this character to the next.
         */
        Character(char c, TexturePtr t, Vector2f s, Vector2f b, 
            unsigned int a) : char_(c), texture(t), size(s),
            bearing(b), advance(a) {}

        /*!
         * Copy constructor.
         *
         * \param c The character to copy.
         */
        Character(Character& c) : char_(c.char_), texture(c.texture),
          size(c.size), bearing(c.bearing), advance(c.advance){}

        /*!
         * Move constructor.
         *
         * \param c The character to move.
         */
        Character(Character&& c) : char_(c.char_), texture(c.texture),
          size(c.size), bearing(c.bearing), advance(c.advance){}

        char char_; //!< The character that this object represents.
        TexturePtr texture; //!< The texture used to draw this character.
        Vector2f size; //!< The size of this character's glyph.
        Vector2f bearing; //!< The bearing of this character's glyph.
        unsigned int advance; //!< The advance amount for this character.
    };

  } // namespace graphics
} // namespace cannon


#endif /* ifndef CANNON_GRAPHICS_CHARACTER_H */
