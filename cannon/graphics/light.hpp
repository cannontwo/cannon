#ifndef CANNON_GRAPHICS_LIGHT_H
#define CANNON_GRAPHICS_LIGHT_H 

#include <Eigen/Dense>

#include <cannon/graphics/geometry/drawable_geom.hpp>

using namespace Eigen;

namespace cannon {
  namespace graphics {

    class Light {
      public:
        Light() = delete;

        Light(Vector4f ambient, Vector4f diffuse, Vector4f specular) :
          ambient_(ambient), diffuse_(diffuse), specular_(specular) {}

        virtual ~Light() {};

        virtual void apply(const std::shared_ptr<geometry::DrawableGeom> geom) const {
          geom->program->set_uniform("light.ambient", (Vector4f)(ambient_ * std::pow(2.0, intensity_)));
          geom->program->set_uniform("light.diffuse", (Vector4f)(diffuse_ * std::pow(2.0, intensity_)));
          geom->program->set_uniform("light.specular", (Vector4f)(specular_ * std::pow(2.0, intensity_)));
          geom->program->set_uniform("light.position", pos_);
        }

        void set_ambient(const Vector4f& ambient) {
          ambient_ = ambient;
        }
        
        void set_diffuse(const Vector4f& diffuse) {
          diffuse_ = diffuse;
        }

        void set_specular(const Vector4f& specular) {
          specular_ = specular;
        }

        void set_pos(const Vector4f& pos) {
          pos_ = pos;
        }

      protected:
        Vector4f ambient_;
        Vector4f diffuse_;
        Vector4f specular_;
        Vector4f pos_ = Vector4f::Zero();

        float intensity_ = 0.0;

    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_LIGHT_H */
