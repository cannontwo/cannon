#pragma once
#ifndef CANNON_GRAPHICS_CAMERA_H
#define CANNON_GRAPHICS_CAMERA_H 

/*!
 * \file cannon/graphics/camera.hpp
 * \brief File containing Camera class definition.
 */

/*!
 * \namespace cannon::graphics
 * \brief Namespace containing graphics classes and functions. Many of these are
 * wrappers around OpenGL functionality.
 */

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace graphics {

    /*!
     * \brief Class representing a simple 3D camera able to translate and rotate.
     * 
     * This class implements a simple, fly-style camera. It is adapted from
     * https://learnopengl.com/Getting-started/Camera. The camera is
     * responsible for generating appropriate perspective matrices for OpenGL
     * rendering and is held by graphics classes that provide 3D rendering capabilities.
     *
     * \sa cannon::graphics::Viewer3D, cannon::graphics::DeferredRenderer
     */
    class Camera {
      public:
        Camera() = delete;

        /*!
         * Constructor creating a Camera from an initial pose, a target
         * location for the camera to point at, an up vector that is
         * non-colinear with the viewing direction, and optionally a movement
         * speed for the camera.
         * 
         * \param pos The initial camera position.
         * \param target The initial location for the camera to point at.
         * \param up A vector representing the camera's local "up" direction.
         * \param speed The initial camera movement speed.
         *
         */
        Camera(const Vector3f& pos, const Vector3f& target, const Vector3f& up, float speed=0.05) : pos_(pos), direction_(pos - target), up_(up), speed_(speed) {
          direction_.normalize();
          up_.normalize();

          right_ = up_.cross(direction_);
          right_.normalize();
        }

        /*!
         * Method returning the view matrix that this camera represents. For
         * more information on computing this matrix, see
         * https://learnopengl.com/Getting-started/Camera
         *
         * \returns The view matrix.
         */
        Matrix4f get_view_mat() const;

        /*! 
         * Method returning the camera position.
         * 
         * \returns The camera position vector.
         */
        Vector3f get_pos() const;

        /*!
         * Method returning the camera facing direction.
         * 
         * \returns The camera facing direction as a vector.
         */
        Vector3f get_direction() const;

        /*!
         * Method returning the camera-local "right" vector, which is
         * orthogonal to the camera facing direction and "up" direction.
         * 
         * \returns The "right" vector.
         */
        Vector3f get_right() const;

      
        /*!
         * Method to set the camera movement speed.
         * 
         * \param speed The new camera speed.
         */
        void set_speed(float speed);

        /*! 
         * Method to set the camera base speed. This is the camera speed when
         * un-modified by "sprinting".
         * 
         * \param base_speed The new base camera speed.
         */
        void set_base_speed(float base_speed);

        // Controls
        /*!
         * Method to cause the camera to move forward. "Forward" is defined by
         * the camera moving direction, and the amount that the camera position
         * is modified is defined by the camera speed.
         * 
         * \sa cannon::graphics::Camera::move_forward
         */
        void move_forward();

        /*!
         * Method to cause the camera to move backward. "Backward" is defined
         * by negation of the camera facing direction.
         *
         * \sa cannon::graphics::Camera::move_forward
         */
        void move_backward();

        /*!
         * Method to cause the camera to strafe left. "Left" is defined by
         * the negation of the camera's right vector.
         * 
         * \sa cannon::graphics::Camera::move_forward
         */
        void strafe_left();

        /*!
         * Method to cause the camera to strafe right. "Right" is defined by
         * the camera's right vector.
         * 
         * \sa cannon::graphics::Camera::move_forward
         */
        void strafe_right();

        /*!
         * Method to cause the camera to continuously move forward, or to cause
         * the camera to stop continuously moving forward. This method is
         * useful for moving the camera in response to user input, as it
         * reduces over-registration of keyboard input and can lead to more
         * consistent movement speed.
         * 
         * \param on Whether the camera should move forward or cease moving forward.
         */
        void set_move_forward(bool on);

        /*!
         * Method to cause the camera to continuously move backward (or stop moving backward).
         * 
         * \param on Whether the camera should move backward or cease moving backward. 
         * 
         * \sa cannon::graphics::Camera::set_move_forward
         */
        void set_move_backward(bool on);

        /*!
         * Method to cause the camera to continuously move left (or stop moving left).
         * 
         * \param on Whether the camera should move left or cease moving left. 
         * 
         * \sa cannon::graphics::Camera::set_move_forward
         */
        void set_strafe_left(bool on);

        /*!
         * Method to cause the camera to continuously move right (or stop moving right).
         * 
         * \param on Whether the camera should move right or cease moving right. 
         * 
         * \sa cannon::graphics::Camera::set_move_forward
         */
        void set_strafe_right(bool on);


        /*!
         * Method to cause the camera to update its position. This is primarily
         * used in concert with the set_move_* and set_strafe_* methods.
         */
        void update_pos();

        
        /*!
         * Method to set the camera position.
         * 
         * \param pos The new camera position.
         */
        void set_pos(const Vector3f& pos);

        /*!
         * Method to set the camera facing direction.
         *
         * \param direction The new camera direction, which should be of unit length.
         */
        void set_direction(const Vector3f& direction);

      private:
        Vector3f pos_; //!< The camera position
        Vector3f direction_; //!< The camera facing direction
        Vector3f up_; //!< The camera-local up vector
        Vector3f right_; //!< The camera-local right vector

        bool move_forward_ = false; //!< Whether the camera is moving continuously forward
        bool move_backward_ = false; //!< Whether the camera is moving continuously backward
        bool strafe_right_ = false; //!< Whether the camera is moving continuously right
        bool strafe_left_ = false; //!< Whether the camera is moving continuously left

        float speed_ = 1.0; //!< The current camera movement speed
        float base_speed_ = 0.5; //!< The base camera movement speed
    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_CAMERA_H */
