#ifndef CANNON_GRAPHICS_RANDOM_COLOR_H
#define CANNON_GRAPHICS_RANDOM_COLOR_H 

/*!
 * \file cannon/graphics/random_color.hpp
 * File containing utility functions relating to generating random colors.
 */

#include <cmath>
#include <random>

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace graphics {

    /*!
     * Generate a random color with a given saturation and value (lightness)
     * and randomly selected hue.
     *
     * \param saturation The saturation of the generated color.
     * \param value The value (or lightness) of the generated color.
     *
     * \returns A vector representing the generated color. Note that the fourth
     * entry in this vector will always be 1.
     */
    Vector4f generate_random_color(float saturation=0.75f, float value=.90f);

    /*!
     * Convert the hue-saturation-value representation of a color into an
     * equivalent RGB representation.
     *
     * \param hue The hue of the color. Should be between 0 and 360.
     * \param saturation The saturation of the color. Should be between 0 and 1.
     * \param value The value of the color. Should be between 0 and 1.
     *
     * \returns An equivalent RGB color.
     */
    Vector3f hsv_to_rgb(float hue, float saturation, float value);

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_RANDOM_COLOR_H */
