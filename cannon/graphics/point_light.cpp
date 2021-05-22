#include <cannon/graphics/point_light.hpp>

#include <cannon/graphics/shader_program.hpp>

using namespace cannon::graphics;

void PointLight::apply(const std::shared_ptr<geometry::DrawableGeom> geom) const {
  geom->program->set_uniform("light.ambient", (Vector4f)(ambient_ * std::pow(2.0, intensity_)));
  geom->program->set_uniform("light.diffuse", (Vector4f)(diffuse_ * std::pow(2.0, intensity_)));
  geom->program->set_uniform("light.specular", (Vector4f)(specular_ * std::pow(2.0, intensity_)));
  geom->program->set_uniform("light.position", pos_);

  geom->program->set_uniform("light.constant", constant_);
  geom->program->set_uniform("light.linear", linear_);
  geom->program->set_uniform("light.quadratic", quadratic_);
}

void PointLight::apply(const std::shared_ptr<ShaderProgram> s, int idx) const {
  std::string preamble("point_lights[");
  preamble += std::to_string(idx);
  preamble += "]";

  s->set_uniform(preamble + ".ambient", (Vector4f)(ambient_ * std::pow(2.0, intensity_)));
  s->set_uniform(preamble + ".diffuse", (Vector4f)(diffuse_ * std::pow(2.0, intensity_)));
  s->set_uniform(preamble + ".specular", (Vector4f)(specular_ * std::pow(2.0, intensity_)));
  s->set_uniform(preamble + ".position", pos_);

  s->set_uniform(preamble + ".constant", constant_);
  s->set_uniform(preamble + ".linear", linear_);
  s->set_uniform(preamble + ".quadratic", quadratic_);
}
