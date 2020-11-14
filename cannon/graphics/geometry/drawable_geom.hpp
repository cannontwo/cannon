#ifndef CANNON_GRAPHICS_GEOMETRY_DRAWABLE_GEOM_H
#define CANNON_GRAPHICS_GEOMETRY_DRAWABLE_GEOM_H 

#include <Eigen/Dense>

#include <cannon/graphics/projection.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <imgui.h>

using namespace Eigen;

namespace cannon {
  namespace graphics {
    namespace geometry {

      struct Material {
        Material(const Vector4f& a, const Vector4f& d, const Vector4f& s, float sh) 
          : ambient(a), diffuse(d), specular(s), shininess(sh) {}

        Vector4f ambient;
        Vector4f diffuse;
        Vector4f specular;
        float shininess;
      };

      class DrawableGeom {
        public:
          virtual void draw(const Matrix4f& view, const Matrix4f& perspective) const = 0;
          virtual void draw(std::shared_ptr<ShaderProgram> p, const Matrix4f&
              view, const Matrix4f& perspective) const = 0;

          void set_material(const Material& material) {
            material_ = material;
          }

          virtual Matrix4f get_model_mat() const {
            Affine3f trans;
            trans = AngleAxisf(to_radians(0.0f), Vector3f::UnitX()) *
              Translation3f(pos_) * rot_ * Scaling(scale_);

            return trans.matrix();
          }

          Matrix4f get_normal_mat() const {
            Matrix4f model = get_model_mat();
            Matrix4f tmp = model.inverse().transpose();

            // Remove scaling/translation
            tmp(0, 3) = 0.0;
            tmp(1, 3) = 0.0;
            tmp(2, 3) = 0.0;
            tmp(3, 3) = 1.0;
            tmp(3, 0) = 0.0;
            tmp(3, 1) = 0.0;
            tmp(3, 2) = 0.0;

            return tmp;
          }

          virtual void set_pos(const Vector3f& pos) {
            pos_ = pos;
          }

          virtual void set_rot(const AngleAxisf& rot) {
            rot_ = rot;
          }

          virtual void set_scale(const float scale) {
            scale_ = scale;
          }

          virtual void write_imgui(int idx) {
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

          std::shared_ptr<ShaderProgram> program;

        protected:
          virtual ~DrawableGeom() {};

          Material material_ = {
            Vector4f{0.0, 0.0, 0.0, 1.0},
            Vector4f{0.0, 0.0, 0.0, 1.0},
            Vector4f{0.0, 0.0, 0.0, 1.0},
            32.0
          };
          Matrix4f model_mat_;

          Vector3f pos_ = Vector3f::Zero();
          AngleAxisf rot_ = AngleAxisf(0.0, Vector3f::UnitZ());
          float scale_ = 1.0;

          std::string name_ = std::string("Drawable");
      };

    } // namespace geometry
  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_GEOMETRY_DRAWABLE_GEOM_H */
