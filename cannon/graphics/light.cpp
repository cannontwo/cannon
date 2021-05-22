#include <cannon/graphics/light.hpp>

#include <cannon/graphics/shader_program.hpp>

using namespace cannon::graphics;

void Light::apply(const std::shared_ptr<geometry::DrawableGeom> geom) const {
  geom->program->set_uniform("light.ambient", (Vector4f)(ambient_ * std::pow(2.0, intensity_)));
  geom->program->set_uniform("light.diffuse", (Vector4f)(diffuse_ * std::pow(2.0, intensity_)));
  geom->program->set_uniform("light.specular", (Vector4f)(specular_ * std::pow(2.0, intensity_)));
  geom->program->set_uniform("light.position", pos_);
}
