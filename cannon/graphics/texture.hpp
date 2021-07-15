#pragma once
#ifndef CANNON_GRAPHICS_TEXTURE_H
#define CANNON_GRAPHICS_TEXTURE_H 

/*!
 * \file cannon/graphics/texture.hpp
 * \brief File containing Texture class definition.
 */

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace cannon {
  namespace graphics {

    /*!
     * \brief Class representing an OpenGL texture, which may or may not be
     * loaded from a file on disk.
     */
    class Texture {
      public:

        /*!
         * \brief Constructor taking optional width, height, internal format, data
         * type, format, data, and texture unit arguments with sensible
         * defaults.
         *
         * This constructor is primarily for use with Framebuffer.
         */
        Texture(int width = 800, int height = 600,
                GLint internal_format = GL_RGBA,
                GLenum data_type = GL_UNSIGNED_BYTE, GLenum format = GL_RGBA,
                const void *data = NULL, GLenum texture_unit = GL_TEXTURE0)
            : internal_format(internal_format), format_(format),
              data_type(data_type), width_(width), height_(height),
              data_((unsigned char *)data), gl_texture_unit_(texture_unit) {

          glGenTextures(1, &gl_texture_);

          glBindTexture(GL_TEXTURE_2D, gl_texture_);
          glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width_, height_, 0, format_, data_type, data);

          set_wrap_repeat();
          set_filter_linear();

          unbind();
        }

        /*!
         * \brief Constructor taking the path of an image to load and store in this
         * texture, as well as optional arguments specifying whether to load
         * alpha channels, the texture unit to use, and whether to flip the
         * loaded image's Y-Axis. 
         */
        Texture(const std::string& path, bool use_alpha=false, GLenum
            texture_unit=GL_TEXTURE0, bool flip=true) : path(path), gl_texture_unit_(texture_unit) {
          stbi_set_flip_vertically_on_load(flip);
          data_ = stbi_load(path.c_str(), &width_, &height_, &num_channels_, 0);
          if (data_ == nullptr) 
            throw std::runtime_error("Could not load image for texture");

          glGenTextures(1, &gl_texture_);
          bind(texture_unit);

          GLenum format;
          GLenum pix_format;
          if (num_channels_ == 1) {
            format = GL_RED;
            pix_format = GL_RED;
          } else if (num_channels_ == 3) {
            format = GL_SRGB;
            //format = GL_RGB;
            pix_format = GL_RGB;
          } else if (num_channels_ == 4) {
            format = GL_SRGB_ALPHA;
            //format = GL_RGBA;
            pix_format = GL_RGBA;
          } else
            throw std::runtime_error("Unrecognized texture format");

          glTexImage2D(GL_TEXTURE_2D, 0, format, width_, height_, 0, pix_format, GL_UNSIGNED_BYTE, data_);

          set_wrap_repeat();
          set_filter_linear();

          bind(texture_unit);
          glGenerateMipmap(GL_TEXTURE_2D);

          stbi_image_free(data_);

          unbind();
        }

        /*!
         * \brief Constructor taking a FreeType font face to load and optional texture
         * unit to use. This is primarily for use with cannon::graphics::Font.
         */
        Texture(FT_Face &face, GLenum texture_unit=0) :
            width_(face->glyph->bitmap.width), height_(face->glyph->bitmap.rows),
            data_(face->glyph->bitmap.buffer), gl_texture_unit_(texture_unit) {
          glGenTextures(1, &gl_texture_);
          bind(texture_unit);

          set_wrap_clamp_edge();
          set_filter_linear();

          bind(texture_unit);
          glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width_, height_, 0, GL_RED, GL_UNSIGNED_BYTE, data_);

          unbind();
        }

        /*!
         * \brief Destructor. Frees allocated OpenGL texture.
         */
        ~Texture() {
          if (glfwGetCurrentContext() != NULL) {
            glDeleteTextures(1, &gl_texture_);
          }
        }

        /*!
         * \brief Static function to get vector of all loaded textures. Can be used to
         * prevent unnecessary reloading.
         *
         * \returns Vector of pointers to loaded textures.
         */
        static std::vector<std::shared_ptr<Texture>>& get_loaded_textures() {
          static std::vector<std::shared_ptr<Texture>> loaded_textures;
          return loaded_textures;
        }

        /*!
         * \brief Static function to load a texture from a path.
         *
         * \param path The path of an image to load into a texture.
         *
         * \returns A pointer to the loaded Texture object.
         */
        static std::shared_ptr<Texture> load_texture(const std::string& path) {
          for (auto& tex : get_loaded_textures()) {
            if (path.compare(tex->path) == 0) {
              return tex;
            }
          }

          // We need to actually create the new texture
          auto ret_tex = std::make_shared<Texture>(path, false, GL_TEXTURE0, false);
          get_loaded_textures().push_back(ret_tex);
          return ret_tex;
        }

        /*!
         * \brief Method to bind this texture to texture unit 0.
         *
         * Sets GL_TEXTURE_BINDING_2D to gl_texture_ and GL_ACTIVE_TEXTURE to GL_TEXTURE0.
         */
        void bind() const;

        /*!
         * \brief Method to bind this texture to the input texture unit.
         *
         * Sets GL_TEXTURE_BINDING_2D to gl_texture_ and GL_ACTIVE_TEXTURE to texture_unit.
         *
         * \param texture_unit The texture unit to bind to.
         */
        void bind(GLenum texture_unit) const;

        /*!
         * \brief Method to unbind this texture from texture unit 0.
         *
         * Sets GL_TEXTURE_BINDING_2D to 0 and GL_ACTIVE_TEXTURE to GL_TEXTURE0.
         */
        void unbind() const;

        /*!
         * \brief Method to unbind this texture from the input texture unit.
         *
         * Sets GL_TEXTURE_BINDING_2D to 0 and GL_ACTIVE_TEXTURE to texture_unit.
         *
         * \param texture_unit The texture unit to unbind.
         */
        void unbind(GLenum texture_unit) const;

        /*!
         * \brief Method to bind this texture to the current framebuffer at the input
         * color attachment.
         *
         * Does not affect OpenGL state.
         *
         * \param attachment The color attachment to attach this texture to.
         */
        void framebuffer_bind(GLenum attachment = GL_COLOR_ATTACHMENT0) const;

        /*!
         * \brief Set the edge sampling behavior on this texture to wrap repeat.
         *
         * Does not affect OpenGL state.
         */
        void set_wrap_repeat();

        /*!
         * \brief Set the edge sampling behavior on this texture to wrap mirrored.
         *
         * Does not affect OpenGL state.
         */
        void set_wrap_mirrored_repeat();

        /*!
         * \brief Set the edge sampling behavior on this texture to clamp to the edge
         * color.
         *
         * Does not affect OpenGL state.
         */
        void set_wrap_clamp_edge();

        /*!
         * \brief Set the edge sampling behavior on this texture to clamp to the border color.
         *
         * Does not affect OpenGL state.
         */
        void set_wrap_clamp_border();

        /*!
         * \brief Set the filtering behavior on this texture to linear.
         *
         * Does not affect OpenGL state.
         */
        void set_filter_linear();

        /*!
         * \brief Set the filtering behavior on this texture to nearest.
         *
         * Does not affect OpenGL state.
         */
        void set_filter_nearest();

        /*!
         * \brief Write ImGui controls for this texture.
         *
         * Does not affect OpenGL state.
         */
        void write_imgui();

        /*!
         * \brief Re-buffer data to GPU.
         */
        void buffer();

        std::string path; //!< Path that this texture was loaded from.

        friend class Framebuffer;

        GLint internal_format; //!< Internal format used by OpenGL to store texture.
        GLenum format_; //!< Format of the source image to be converted.
        GLenum data_type; //!< Color information data type for this texture.

      private:
        int width_; //!< Texture width
        int height_; //!< Texture height
        int num_channels_; //!< Number of color channels for this texture
        unsigned char *data_; //!< Texture image data

        GLuint gl_texture_; //!< OpenGL texture object
        GLenum gl_texture_unit_; //!< OpenGL texture unit to bind this texture to
    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_TEXTURE_H */
