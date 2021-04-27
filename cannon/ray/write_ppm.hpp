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

  }
}

#endif /* ifndef CANNON_RAY_WRITE_PPM */
