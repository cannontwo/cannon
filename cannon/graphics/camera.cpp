#include <cannon/graphics/camera.hpp>

using namespace cannon::graphics;

Matrix4f Camera::get_view_mat() const {
  // The "LookAt" matrix
  Matrix4f rot_mat;
  Matrix4f trans_mat;

  Vector3f tmp_dir = direction_;
  tmp_dir.normalize();

  Vector3f camera_up = tmp_dir.cross(right_);

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

Vector3f Camera::get_pos() const {
  return pos_;
}

Vector3f Camera::get_direction() const {
  return direction_;
}

Vector3f Camera::get_right() const {
  return right_;
}

void Camera::set_speed(float speed) {
  speed_ = speed * base_speed_;
}

void Camera::set_base_speed(float base_speed) {
  base_speed_ = base_speed;
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

void Camera::set_move_forward(bool on) {
  move_forward_ = on;
}

void Camera::set_move_backward(bool on) {
  move_backward_ = on;
}

void Camera::set_strafe_left(bool on) {
  strafe_left_ = on;
}

void Camera::set_strafe_right(bool on) {
  strafe_right_ = on;
}

void Camera::update_pos() {
  if (move_forward_)
    move_forward();

  if (move_backward_)
    move_backward();
  
  if (strafe_left_)
    strafe_left();

  if (strafe_right_)
    strafe_right();
}

void Camera::set_direction(const Vector3f& direction) {
  direction_ = direction;
  direction_.normalize();

  right_ = up_.cross(direction_);
  right_.normalize();
}
