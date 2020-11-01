#ifndef CANNON_GRAPHICS_GEOMETRY_DRAWABLE_GEOM_H
#define CANNON_GRAPHICS_GEOMETRY_DRAWABLE_GEOM_H 

#include <Eigen/Dense>

#include <cannon/graphics/projection.hpp>

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

          void set_material(const Material& material) {
            material_ = material;
          }

          Matrix4f get_model_mat() const {
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

          void set_pos(const Vector3f& pos) {
            pos_ = pos;
          }

          void set_rot(const AngleAxisf& rot) {
            rot_ = rot;
          }

          void set_scale(const float scale) {
            scale_ = scale;
          }

        protected:
          virtual ~DrawableGeom() {};

          Material material_ = {
            Vector4f{1.0, 0.5, 0.31, 1.0},
            Vector4f{1.0, 0.5, 0.31, 1.0},
            Vector4f{0.5, 0.5, 0.5, 1.0},
            32.0
          };
          Matrix4f model_mat_;

          Vector3f pos_;
          AngleAxisf rot_;
          float scale_ = 1.0;
      };

    } // namespace geometry
  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_GEOMETRY_DRAWABLE_GEOM_H */
