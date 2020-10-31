#ifndef CANNON_GRAPHICS_CAMERA_H
#define CANNON_GRAPHICS_CAMERA_H 

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace graphics {

    class Camera {
      public:
        Camera() = delete;

        Camera(const Vector3f& pos, const Vector3f& target, const Vector3f& up, float speed=0.05) : pos_(pos), direction_(pos - target), speed_(speed) {
          direction_.normalize();
          up_.normalize();

          right_ = up.cross(direction_);
          right_.normalize();

          up_ = direction_.cross(right_);
        }

        Matrix4f get_view_mat();
        void set_speed(float speed);

        // Controls
        void move_forward();
        void move_backward();
        void strafe_left();
        void strafe_right();

      private:
        Vector3f pos_;
        Vector3f direction_;
        Vector3f up_;
        Vector3f right_;

        float speed_;
    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_CAMERA_H */
