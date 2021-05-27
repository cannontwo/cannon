#pragma once
#ifndef CANNON_RAY_FILM_H
#define CANNON_RAY_FILM_H 

/*!
 * \file cannon/ray/film.hpp
 * \brief File containing Film and FilmTile class definitions.
 */

#include <memory>
#include <mutex>
#include <vector>

#include <Eigen/Dense>

#include <cannon/ray/write_ppm.hpp>
#include <cannon/utils/class_forward.hpp>

using namespace Eigen;

namespace cannon {
  namespace ray {

    CANNON_CLASS_FORWARD(Filter);

    /*!
     * \brief Struct representing a single tile in the image film. Accumulates
     * contributions from image samples.
     */
    struct FilmPixel {
      Vector3d color_sum_ = Vector3d::Zero(); //!< Sum of color samples for this pixel
      double filter_weight_sum_ = 0.0; //!< Sum of filter weights for this pixel
    };

    /*!
     * \brief Struct representing a tile of pixels in the film which can be
     * independently processed.
     */
    struct FilmTile {

      /*!
       * \brief Add a sample to this film tile.
       *
       * \param p_film Point on film that sample hits.
       * \param color Color of sample
       */
      void add_sample(const Vector2d& p_film, const Vector3d& color);

      /*!
       * \brief Get pixel in this tile at location (i, j)
       *
       * \param i Horizontal coordinate of pixel
       * \param j Vertical coordinate of pixel
       *
       * \returns Reference to pixel.
       */
      FilmPixel& get_pixel(int i, int j);

      unsigned int origin_x_, origin_y_; //!< Origin of this tile
      unsigned int extent_x_, extent_y_; //!< Extent of this tile
      Vector2d filter_radius_; //!< Filter radius for film
      Vector2d inv_filter_radius_; //!< 1 / filter_radius
      const double *filter_table_; //!< Filter table for film
      int filter_table_width_;
      std::vector<FilmPixel> pixels_; //!< Pixels in this tile
    };

    /*!
     * \brief Class representing Film to which an image is rendered.
     */
    class Film {
      public:

        /*!
         * \brief Constructor.
         */
        Film(int width, int height, int tile_size, std::unique_ptr<Filter>
            filter);

        /*!
         * Get tile (i, j) of this film.
         */
        std::unique_ptr<FilmTile> get_film_tile(int i, int j) const;

        /*!
         * Merge input film tile into the final image.
         *
         * \param tile The tile to merge.
         */
        void merge_film_tile(std::unique_ptr<FilmTile> tile);

        /*!
         * Write this film to the input file.
         *
         * \param filename The filename to write this film to in PPM format.
         * \param samples The number of samples performed per pixel
         */
        void write_image(const std::string& filename, int samples);

      public:
        int width_, height_; //!< Width and height of film
        int tile_size_; //!< Size of each film tile
        std::mutex mut_; //!< Mutex controlling image data writing/reading
        std::vector<FilmPixel> pixels_; //!< Rendered image data
        std::unique_ptr<Filter> filter_; //!< Image reconstruction filter

        static constexpr int filter_table_width_ = 16; //!< Size of cached filter table
        double filter_table_[filter_table_width_ * filter_table_width_]; //!< Cached filter table
        
    };

  } // namespace ray
} // namespace cannon

#endif /* ifndef CANNON_RAY_FILM_H */
