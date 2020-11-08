#ifndef CANNON_GRAPHICS_TEXTURE_H
#define CANNON_GRAPHICS_TEXTURE_H 

#include <string>
#include <stdexcept>

#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <cannon/log/registry.hpp>

using namespace cannon::log;

namespace cannon {
  namespace graphics {

    class Texture {
      public:
        Texture() = delete;

        Texture(const std::string& path, bool use_alpha=false, GLenum
            texture_unit=GL_TEXTURE0) : path(path), gl_texture_unit_(texture_unit) {
          stbi_set_flip_vertically_on_load(true);
          data_ = stbi_load(path.c_str(), &width_, &height_, &num_channels_, 0);
          if (data_ == nullptr) 
            throw std::runtime_error("Could not load image for texture");

          glGenTextures(1, &gl_texture_);
          bind(texture_unit);

          set_wrap_repeat();
          set_filter_linear();

          GLenum format;
          GLenum pix_format;
          if (num_channels_ == 1) {
            format = GL_RED;
            pix_format = GL_RED;
          } else if (num_channels_ == 3) {
            //format = GL_SRGB;
            format = GL_RGB;
            pix_format = GL_RGB;
          } else if (num_channels_ == 4) {
            //format = GL_SRGB_ALPHA;
            format = GL_RGBA;
            pix_format = GL_RGBA;
          } else
            throw std::runtime_error("Unrecognized texture format");

          log_info("Loading texture in", path);

          glTexImage2D(GL_TEXTURE_2D, 0, format, width_, height_, 0, pix_format, GL_UNSIGNED_BYTE, data_);

          set_wrap_repeat();
          set_filter_linear();

          glGenerateMipmap(GL_TEXTURE_2D);

          stbi_image_free(data_);
        }

        Texture(FT_Face &face, GLenum texture_unit=0) :
            width_(face->glyph->bitmap.width), height_(face->glyph->bitmap.rows),
            data_(face->glyph->bitmap.buffer), gl_texture_unit_(texture_unit) {
          glGenTextures(1, &gl_texture_);
          bind(texture_unit);

          set_wrap_clamp_edge();
          set_filter_linear();

          glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width_, height_, 0, GL_RED, GL_UNSIGNED_BYTE, data_);
        }

        static std::vector<std::shared_ptr<Texture>>& get_loaded_textures() {
          static std::vector<std::shared_ptr<Texture>> loaded_textures;
          return loaded_textures;
        }

        void bind() const;
        void bind(GLenum texture_unit) const;
        void unbind() const;

        void set_wrap_repeat();
        void set_wrap_mirrored_repeat();
        void set_wrap_clamp_edge();
        void set_wrap_clamp_border();

        void set_filter_linear();
        void set_filter_nearest();

        std::string path;


      private:
        int width_;
        int height_;
        int num_channels_;
        unsigned char *data_;

        unsigned int gl_texture_;
        GLenum gl_texture_unit_;
    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_TEXTURE_H */
