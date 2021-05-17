#pragma once
#ifndef CANNON_GRAPHICS_PROJECTION_H
#define CANNON_GRAPHICS_PROJECTION_H 

/*!
 * \file cannon/graphics/projection.hpp
 * \brief File containing utility free functions for computing projections.
 */

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace graphics {

    /*!
     * Function to convert degrees to radians.
     * 
     * \param degrees Angle representation in degrees.
     *
     * \returns Angle representation in radians.
     */
    float to_radians(float degrees);

    /*!
     * Compute perspective matrix corresponding to a view frustum specified by
     * the inputs.
     *
     * \param left Left extent of the frustum.
     * \param right Right extent of the frustum.
     * \param bottom Lower extent of the frustum.
     * \param top Upper extent of the frustum.
     * \param near Depth of the near clipping plane.
     * \param far Depth of the far clipping plane.
     *
     * \returns The corresponding perspective matrix.
     */
    Matrix4f make_perspective_frustum(float left, float right, float bottom,
        float top, float near, float far);

    /*!
     * Compute perspective matrix corresponding to a view frustum with a given
     * field of view and aspect ratio.
     *
     * \param fov The field of view (in radians) of the frustum.
     * \param aspect The aspect ratio of the frustum.
     * \param near Depth of the near clipping plane.
     * \param far Depth of the far clipping plane.
     *
     * \returns The corresponding perspective matrix.
     */
    Matrix4f make_perspective_fov(float fov, float aspect, float near, float far);

    /*!
     * Compute orthographic projection matrix corresponding to a view frustum
     * specified by the inputs.
     *
     * \param left Left extent of the frustum.
     * \param right Right extent of the frustum.
     * \param bottom Lower extent of the frustum.
     * \param top Upper extent of the frustum.
     * \param near Depth of the near clipping plane.
     * \param far Depth of the far clipping plane.
     *
     * \returns The corresponding projection matrix.
     */
    Matrix4f make_orthographic(float left, float right, float bottom, float
        top, float near, float far);

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_PROJECTION_H */
