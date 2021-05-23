#pragma once
#ifndef CANNON_RAY_FILM_H
#define CANNON_RAY_FILM_H 

/*!
 * \file cannon/ray/film.hpp
 * \brief File containing Film and FilmTile class definitions.
 */

#include <memory>
#include <mutex>

#include <Eigen/Dense>

#include <cannon/ray/write_ppm.hpp>

using namespace Eigen;

namespace cannon {
  namespace ray {

    /*!
     * \brief Struct representing a tile of pixels in the film which can be
     * independently processed.
     */
    struct FilmTile {
      unsigned int origin_x_, origin_y_; //!< Origin of this tile
      unsigned int extent_x_, extent_y_; //!< Extent of this tile
      MatrixXd data_; //!< Tile pixel color data
    };

    /*!
     * \brief Class representing Film to which an image is rendered.
     */
    class Film {
      public:

        Film(int width, int height, int tile_size) : width_(width),
        height_(height), tile_size_(tile_size),
        image_data_(MatrixXd::Zero(3, width*height)) {}

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
        MatrixXd image_data_; //!< Rendered image data

    };

  } // namespace ray
} // namespace cannon

#endif /* ifndef CANNON_RAY_FILM_H */
