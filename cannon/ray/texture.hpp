#pragma once
#ifndef CANNON_RAY_TEXTURE_H
#define CANNON_RAY_TEXTURE_H 

/*!
 * \file cannon/ray/texture.hpp
 * \brief File containing Texture and subclasses class definitions.
 */

#include <memory>

#include <Eigen/Dense>
#include <stb_image/stb_image.h>

#include <cannon/math/perlin.hpp>

using namespace Eigen;

using namespace cannon::math;

namespace cannon {
  namespace ray {

    /*!
     * \brief Abstract class representing a texture on some geometry.
     */
    class Texture {
      public:
        /*!
         * Method to get the value of this texture at U,V surface coordinates.
         *
         * \param u Horizontal surface coordinate
         * \param v Vertical surface coordinate
         * \param p Point being textured
         *
         * \returns Color of texture at point with surface coords
         */
        virtual Vector3d value(double u, double v, const Vector3d& p) const = 0;
    };

    /*!
     * \brief Class representing a texture of a single, fixed color.
     */
    class SolidColor : public Texture {
      public:
        /*!
         * Default constructor.
         */
        SolidColor() {}

        /*!
         * Constructor taking the color to make this texture.
         */
        SolidColor(const Vector3d& c) : color_(c) {}

        /*!
         * Constructor taking RGB components for the color of this texture.
         */
        SolidColor(double red, double green, double blue) : color_(red, green, blue) {}

        /*!
         * Destructor.
         */
        virtual ~SolidColor() {}

        /*!
         * Inherited from Texture.
         */
        virtual Vector3d value(double u, double v, const Vector3d& p) const override {
          return color_;
        }

      private:
        Vector3d color_; //!< Color for this texture
    };

    /*!
     * \brief Class representing a procedural checker texture.
     */
    class CheckerTexture : public Texture {
      public:

        /*!
         * Default constructor.
         */
        CheckerTexture() {}

        /*!
         * Constructor taking textures to alternate in checkerboard pattern.
         */
        CheckerTexture(std::shared_ptr<Texture> even, std::shared_ptr<Texture>
            odd) : even_(even), odd_(odd){}

        /*!
         * Constructor taking colors to alternate in checkerboard pattern.
         */
        CheckerTexture(const Vector3d& c1, const Vector3d& c2) :
          even_(std::make_shared<SolidColor>(c1)),
          odd_(std::make_shared<SolidColor>(c2)) {}

        /*!
         * Destructor.
         */
        virtual ~CheckerTexture() {}

        /*!
         * Inherited from Texture.
         */
        virtual Vector3d value(double u, double v, const Vector3d& p) const override;


      public:
        std::shared_ptr<Texture> even_;
        std::shared_ptr<Texture> odd_;
    };

    /*!
     * \brief Class representing a Perlin noise texture.
     */
    class NoiseTexture : public Texture {
      public:

        /*!
         * Default constructor.
         */
        NoiseTexture() {}

        /*!
         * Constructor taking a noise scale.
         */
        NoiseTexture(double scale) : scale_(scale) {}

        /*!
         * Destructor.
         */
        virtual ~NoiseTexture() {}

        /*!
         * Inherited from Texture.
         */
        virtual Vector3d value(double u, double v, const Vector3d& p) const override {
          return Vector3d(1, 1, 1) * 0.5 * (1.0 + 
              std::sin(scale_ * p.z() + 10*noise_.turbulence(p)));
        }

      public:
        Perlin noise_; //!< Perlin noise generator
        double scale_; //!< Noise scale
    };

    /*!
     * \brief Class representing a texture derived from an image.
     */
    class ImageTexture : public Texture {
      public:

        /*!
         * Default constructor.
         */
        ImageTexture() : data_(nullptr), width_(0), height_(0), bytes_per_scanline_(0) {}

        /*!
         * Constructor taking filename of image to load.
         */
        ImageTexture(const std::string& filename) {
          auto components_per_pixel = bytes_per_pixel;

          data_ = stbi_load(filename.c_str(), &width_, &height_,
              &components_per_pixel, components_per_pixel);

          if (!data_) {
            throw std::runtime_error("Could not open image file for texture.");
          }

          bytes_per_scanline_ = bytes_per_pixel * width_;
        }

        /*!
         * Destructor.
         */
        virtual ~ImageTexture () {}

        /*!
         * Inherited from Texture.
         */
        virtual Vector3d value(double u, double v, const Vector3d& p) const override;

        const static int bytes_per_pixel = 3; //!< Bytes per pixel in this texture

      private:
        unsigned char *data_; //!< Image data
        int width_, height_; //!< Image width, height
        int bytes_per_scanline_; //!< Number of bytes per scanline in image

    };

  } // namespace ray
} // namespace cannon

#endif /* ifndef CANNON_RAY_TEXTURE_H */
