#ifndef CANNON_GRAPHICS_DIRECTIONAL_LIGHT_H
#define CANNON_GRAPHICS_DIRECTIONAL_LIGHT_H

#include <cannon/graphics/light.hpp>

namespace cannon {
  namespace graphics {

    class DirectionalLight : public Light {
      public:
        DirectionalLight() = delete;

        DirectionalLight(Vector4f ambient, Vector4f diffuse, Vector4f specular) :
          Light(ambient, diffuse, specular) {}

        virtual void apply(const std::shared_ptr<geometry::DrawableGeom> geom) const override {
          geom->program->set_uniform("directional_light.ambient", ambient_);
          geom->program->set_uniform("directional_light.diffuse", diffuse_);
          geom->program->set_uniform("directional_light.specular", specular_);
          geom->program->set_uniform("directional_light.direction", direction_);
        }

        void set_direction(const Vector4f& direction) {
          direction_ = direction;
        }

      private:
        Vector4f direction_ = Vector4f::UnitZ();

    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_DIRECTIONAL_LIGHT_H */
