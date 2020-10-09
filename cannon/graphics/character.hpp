#ifndef CANNON_GRAPHICS_CHARACTER_H
#define CANNON_GRAPHICS_CHARACTER_H

#include <Eigen/Dense>

#include <cannon/graphics/texture.hpp>

using namespace Eigen;

namespace cannon {
  namespace graphics {

    class Character {
      public:
        Character() = delete;

        Character(char c, Texture t, Vector2f s, Vector2f b, 
            unsigned int a) : char_(c), texture(t), size(s),
            bearing(b), advance(a) {}

        char char_;
        Texture texture;
        Vector2f size;
        Vector2f bearing;
        unsigned int advance;
    };

  } // namespace graphics
} // namespace cannon


#endif /* ifndef CANNON_GRAPHICS_CHARACTER_H */
