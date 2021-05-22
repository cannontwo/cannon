#include <cannon/graphics/directional_light.hpp>

#include <cannon/graphics/shader_program.hpp>

using namespace cannon::graphics;

void DirectionalLight::apply(const std::shared_ptr<ShaderProgram> s) const {
  s->set_uniform("directional_light.ambient", (Vector4f)(ambient_ * std::pow(2.0, intensity_)));
  s->set_uniform("directional_light.diffuse", (Vector4f)(diffuse_ * std::pow(2.0, intensity_)));
  s->set_uniform("directional_light.specular", (Vector4f)(specular_ * std::pow(2.0, intensity_)));
  s->set_uniform("directional_light.direction", direction_);
}
