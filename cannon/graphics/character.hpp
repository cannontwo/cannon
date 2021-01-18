#ifndef CANNON_GRAPHICS_CHARACTER_H
#define CANNON_GRAPHICS_CHARACTER_H

/*!
 * \file cannon/graphics/character.hpp
 * File containing Character class definition.
 */

#include <Eigen/Dense>

#include <cannon/graphics/texture.hpp>

using namespace Eigen;

namespace cannon {
  namespace graphics {

    class Character {
      public:
        Character() = delete;

        Character(char c, std::shared_ptr<Texture> t, Vector2f s, Vector2f b, 
            unsigned int a) : char_(c), texture(t), size(s),
            bearing(b), advance(a) {}

        Character(Character& c) : char_(c.char_), texture(c.texture),
          size(c.size), bearing(c.bearing), advance(c.advance){}

        Character(Character&& c) : char_(c.char_), texture(c.texture),
          size(c.size), bearing(c.bearing), advance(c.advance){}

        char char_;
        std::shared_ptr<Texture> texture;
        Vector2f size;
        Vector2f bearing;
        unsigned int advance;
    };

  } // namespace graphics
} // namespace cannon


#endif /* ifndef CANNON_GRAPHICS_CHARACTER_H */
