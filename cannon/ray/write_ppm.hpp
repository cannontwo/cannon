#pragma once
#ifndef CANNON_RAY_WRITE_PPM
#define CANNON_RAY_WRITE_PPM

/*!
 * \file cannon/ray/write_ppm.hpp
 * File containing utility functions for working with PPM files.
 */

#include <iostream>

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace ray {

    /*!
     * Function to write a color in PPM format to the input stream.
     *
     * \param os The stream to write this color to.
     * \param pixel_color The RGB color to write in PPM format.
     */
    void write_color(std::ostream& os, const Vector3d& pixel_color);

    /*!
     * Function to write a color in PPM format to the input stream.
     *
     * \param os The stream to write this color to.
     * \param pixel_color The RGB color to write in PPM format.
     * \param samples_per_pixel Number of samples contributing to pixel color.
     */
    void write_color(std::ostream& os, const Vector3d& pixel_color, unsigned int samples_per_pixel);

    /*!
     * Function to write a matrix of all colors in an image to the input stream.
     * 
     * \param os The stream to write this image to.
     * \param pixel_colors Matrix of pixel colors. Should have three rows.
     * \param samples_per_pixel Number of samples per pixel.
     */
    void write_colors(std::ostream& os, const MatrixXd& pixel_colors, unsigned int samples_per_pixel);

  }
}

#endif /* ifndef CANNON_RAY_WRITE_PPM */
