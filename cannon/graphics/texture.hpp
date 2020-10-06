#ifndef CANNON_GRAPHICS_TEXTURE_H
#define CANNON_GRAPHICS_TEXTURE_H 

#include <string>
#include <stdexcept>

#include <glad/glad.h>
#include <stb_image/stb_image.h>

namespace cannon {
  namespace graphics {

    class Texture {
      public:
        Texture() = delete;

        Texture(const std::string& path, bool use_alpha=false, GLenum texture_unit=0) {
          data_ = stbi_load(path.c_str(), &width_, &height_, &num_channels_, 0);
          if (data_ == nullptr) 
            throw std::runtime_error("Could not load image for texture");

          glGenTextures(1, &gl_texture_);
          bind(texture_unit);

          set_wrap_repeat();
          set_filter_linear();

          if (use_alpha)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data_);
          else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, data_);

          glGenerateMipmap(GL_TEXTURE_2D);

          stbi_image_free(data_);
        }

        void bind();
        void bind(GLenum texture_unit);
        void unbind();

        void set_wrap_repeat();
        void set_wrap_mirrored_repeat();
        void set_wrap_clamp_edge();
        void set_wrap_clamp_border();

        void set_filter_linear();
        void set_filter_nearest();

      private:
        int width_;
        int height_;
        int num_channels_;
        unsigned char *data_;

        unsigned int gl_texture_;
    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_TEXTURE_H */
