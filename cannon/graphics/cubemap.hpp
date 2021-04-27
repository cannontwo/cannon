#pragma once
#ifndef CANNON_GRAPHICS_CUBEMAP_H
#define CANNON_GRAPHICS_CUBEMAP_H 

/*!
 * \file cannon/graphics/cubemap.hpp
 * File containing Cubemap class definition.
 */

#include <string>
#include <stdexcept>
#include <vector>
#include <cassert>

#include <glad/glad.h>
#include <stb_image/stb_image.h>

namespace cannon {
  namespace graphics {

    /*!
     * \brief Class representing an OpenGL cubemap.
     *
     * This class encapsulates the various OpenGL objects and state-changing
     * functions necessary to use a cubemap (essentially a collection of six
     * regular textures) in OpenGL. In particular, it can be used for rendering
     * skyboxes. It is adapted from
     * https://learnopengl.com/Advanced-OpenGL/Cubemaps .
     */
    class Cubemap {
      public:

        /*!
         * Constructor that makes an empty cubemap. Does not affect OpenGL state.
         *
         * \param width The width of the faces of the cubemap.
         * \param height The height of the faces of the cubemap.
         * \param channels The number of channels in each face of the cubemap.
         */
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

          unbind();
        }

        /*!
         * Constructor that makes a cubemap by loading the passed textures. 
         *
         * \param face_paths A vector of six strings that should be parsed as
         * texture files in order to initialize the faces of the cubemap.
         */
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

          glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        }


        /*!
         * Binds this cubemap for OpenGL drawing.
         *
         * Sets GL_TEXTURE_CUBE_MAP to gl_cubemap_ and GL_ACTIVE_TEXTURE to
         * texture_unit. 
         *
         * \param texture_unit The texture unit to bind this cubemap to.
         */
        void bind(GLenum texture_unit) const;

        /*!
         * Unbinds this cubemap.
         *
         * Sets GL_TEXTURE_CUBE_MAP to 0 and GL_ACTIVE_TEXTURE to GL_TEXTURE0
         */
        void unbind() const;

        /*!
         * Set the textures making up the faces of this cubemap to repeat on
         * wrap.
         */
        void set_wrap_repeat();

        /*!
         * Set the textures making up the faces of this cubemap to mirror on
         * wrap.
         */
        void set_wrap_mirrored_repeat();

        /*!
         * Set the textures making up the faces of this cubemap to clamp to the
         * edge on wrap.
         */
        void set_wrap_clamp_edge();

        /*!
         * Set the textures making up the faces of this cubemap to clamp to a
         * border color on wrap.
         */
        void set_wrap_clamp_border();

        /*!
         * Set the textures making up the faces of this cubemap to use linear
         * filtering.
         */
        void set_filter_linear();

        /*!
         * Set the textures making up the faces of this cubemap to use nearest
         * value filtering.
         */
        void set_filter_nearest();

      private:
        unsigned int gl_cubemap_; //!< OpenGL object identifier for this cubemap.

    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_CUBEMAP_H */
