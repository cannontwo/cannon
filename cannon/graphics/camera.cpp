#include <cannon/graphics/camera.hpp>

using namespace cannon::graphics;

Matrix4f Camera::get_view_mat() {
  // The "LookAt" matrix
  Matrix4f rot_mat;
  Matrix4f trans_mat;

  Vector3f tmp_dir = direction_;
  tmp_dir.normalize();

  Vector3f camera_up = right_.cross(tmp_dir);

  rot_mat << right_[0], right_[1], right_[2], 0.0,
             camera_up[0], camera_up[1], camera_up[2], 0.0,
             tmp_dir[0], tmp_dir[1], tmp_dir[2], 0.0,
             0.0, 0.0, 0.0, 1.0;

  trans_mat << 1.0, 0.0, 0.0, -pos_[0],
               0.0, 1.0, 0.0, -pos_[1],
               0.0, 0.0, 1.0, -pos_[2],
               0.0, 0.0, 0.0, 1.0;

  return rot_mat * trans_mat;
}

void Camera::set_speed(float speed) {
  speed_ = speed;
}

void Camera::move_forward() {
  pos_ -= speed_ * direction_;
}

void Camera::move_backward() {
  pos_ += speed_ * direction_;
}

void Camera::strafe_left() {
  pos_ -= speed_ * right_;
}

void Camera::strafe_right() {
  pos_ += speed_ * right_;
}

void Camera::set_direction(const Vector3f& direction) {
  direction_ = direction;
  direction_.normalize();

  right_ = up_.cross(direction_);
  right_.normalize();
}
