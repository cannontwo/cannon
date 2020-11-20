#ifndef CANNON_GRAPHICS_CAMERA_H
#define CANNON_GRAPHICS_CAMERA_H 

#include <Eigen/Dense>

#include <cannon/log/registry.hpp>

using namespace Eigen;

using namespace cannon::log;

namespace cannon {
  namespace graphics {

    class Camera {
      public:
        Camera() = delete;

        Camera(const Vector3f& pos, const Vector3f& target, const Vector3f& up, float speed=0.05) : pos_(pos), direction_(pos - target), up_(up), speed_(speed) {
          direction_.normalize();
          up_.normalize();

          right_ = up_.cross(direction_);
          right_.normalize();
        }

        Matrix4f get_view_mat() const;
        Vector3f get_pos() const;
        Vector3f get_direction() const;
        Vector3f get_right() const;

        void set_speed(float speed);
        void set_base_speed(float base_speed);

        // Controls
        void move_forward();
        void move_backward();
        void strafe_left();
        void strafe_right();

        void set_move_forward(bool on);
        void set_move_backward(bool on);
        void set_strafe_left(bool on);
        void set_strafe_right(bool on);

        void update_pos();

        void set_direction(const Vector3f& direction);

      private:
        Vector3f pos_;
        Vector3f direction_;
        Vector3f up_;
        Vector3f right_;

        bool move_forward_ = false;
        bool move_backward_ = false;
        bool strafe_right_ = false;
        bool strafe_left_ = false;

        float speed_ = 1.0;
        float base_speed_ = 0.5;
    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_CAMERA_H */
