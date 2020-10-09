#ifndef CANNON_GRAPHICS_FONT_H
#define CANNON_GRAPHICS_FONT_H 

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

    class Font {
      public:
        Font(bool do_init=true, const std::string&
            path="/usr/share/fonts/truetype/open-sans/OpenSans-Regular.ttf",
            int size=20) : path_(path), size_(size) {
          if (do_init) {
            init();
          }
        }

        Font(Font& f) = delete;
        Font(Font&& f) = delete;

        ~Font() {
          FT_Done_Face(face_);
          FT_Done_FreeType(ft_);
        }

        void init();
        std::shared_ptr<Character> get_char(char c);

      private:
        void load_chars();

        const std::string path_;
        int size_;
        bool initialized_ = false;

        FT_Library ft_;
        FT_Face face_;
        std::map<char, std::shared_ptr<Character>> chars_;

    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_FONT_H */
