#include <cannon/graphics/geometry/drawable_geom.hpp>

#include <imgui.h>

#include <cannon/graphics/projection.hpp>

using namespace cannon::graphics::geometry;

void DrawableGeom::write_imgui(int idx) {
  if (ImGui::TreeNode((name_ + " " + std::to_string(idx)).c_str())) {
    ImGui::ColorEdit3("ambient", material_.ambient.data());
    ImGui::ColorEdit3("diffuse", material_.diffuse.data());
    ImGui::ColorEdit3("specular", material_.specular.data());
    ImGui::SliderFloat3("position", pos_.data(), -10.0, 10.0);
    ImGui::SliderFloat("scale", &scale_, 0.0, 10.0);
    ImGui::SliderAngle("rotation", &rot_.angle());
    ImGui::SliderFloat3("axis", rot_.axis().data(), -1.0, 1.0);
    rot_.axis().normalize();
    ImGui::TreePop();
  }
}

Matrix4f DrawableGeom::get_model_mat() const {
  Affine3f trans;
  trans = AngleAxisf(to_radians(0.0f), Vector3f::UnitX()) *
    Translation3f(pos_) * rot_ * Scaling(scale_);

  return trans.matrix();
}
