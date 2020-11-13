#ifndef CANNON_GRAPHICS_CUBEMAP_H
#define CANNON_GRAPHICS_CUBEMAP_H 

#include <string>
#include <stdexcept>
#include <vector>
#include <cassert>

#include <glad/glad.h>
#include <stb_image/stb_image.h>

namespace cannon {
  namespace graphics {

    class Cubemap {
      public:

        // Empty cubemap
        Cubemap(int width=800, int height=600, int num_channels=3) {

          glGenTextures(1, &gl_cubemap_);
          glBindTexture(GL_TEXTURE_CUBE_MAP, gl_cubemap_);

          for (unsigned int i = 0; i < 6; i++) {
            // Using OpenGL enum packing
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width,
                height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
          }

          set_filter_linear();
          set_wrap_clamp_edge();
        }

        Cubemap(std::vector<std::string> face_paths) {
          assert(face_paths.size() == 6);

          glGenTextures(1, &gl_cubemap_);
          glBindTexture(GL_TEXTURE_CUBE_MAP, gl_cubemap_);

          int width, height, num_channels;
          unsigned char* data;
          for (unsigned int i = 0; i < face_paths.size(); i++) {
            stbi_set_flip_vertically_on_load(false);
            data = stbi_load(face_paths[i].c_str(), &width, &height, &num_channels, 0);
            if (data == nullptr) 
              throw std::runtime_error("Could not load image for texture");
            stbi_set_flip_vertically_on_load(true);

            GLenum format;
            GLenum pix_format;
            if (num_channels == 1) {
              format = GL_RED;
              pix_format = GL_RED;
            } else if (num_channels == 3) {
              format = GL_SRGB;
              //format = GL_RGB;
              pix_format = GL_RGB;
            } else if (num_channels == 4) {
              format = GL_SRGB_ALPHA;
              //format = GL_RGBA;
              pix_format = GL_RGBA;
            } else
              throw std::runtime_error("Unrecognized texture format");

            // Using OpenGL enum packing
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width,
                height, 0, pix_format, GL_UNSIGNED_BYTE, data);

            stbi_image_free(data);
          }
          
          set_filter_linear();
          set_wrap_clamp_edge();

        }

        void bind(GLenum texture_unit) const;
        void unbind() const;

        void set_wrap_repeat();
        void set_wrap_mirrored_repeat();
        void set_wrap_clamp_edge();
        void set_wrap_clamp_border();

        void set_filter_linear();
        void set_filter_nearest();

      private:
        unsigned int gl_cubemap_;

    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_CUBEMAP_H */
