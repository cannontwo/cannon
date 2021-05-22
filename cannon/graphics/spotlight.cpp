#include <cannon/graphics/spotlight.hpp>

#include <cannon/graphics/shader_program.hpp>

using namespace cannon::graphics;

void Spotlight::apply(const std::shared_ptr<geometry::DrawableGeom> geom) const {
  geom->program->set_uniform("light.ambient", (Vector4f)(ambient_ * std::pow(2.0, intensity_)));
  geom->program->set_uniform("light.diffuse", (Vector4f)(diffuse_ * std::pow(2.0, intensity_)));
  geom->program->set_uniform("light.specular", (Vector4f)(specular_ * std::pow(2.0, intensity_)));

  geom->program->set_uniform("light.position", pos_);
  geom->program->set_uniform("light.direction", direction_);
  geom->program->set_uniform("light.cutoff", std::cos(to_radians(cutoff_)));
  geom->program->set_uniform("light.outer_cutoff", std::cos(to_radians(outer_cutoff_)));

  geom->program->set_uniform("light.constant", constant_);
  geom->program->set_uniform("light.linear", linear_);
  geom->program->set_uniform("light.quadratic", quadratic_);
}

void Spotlight::apply(const std::shared_ptr<ShaderProgram> s, int idx) const {
  std::string preamble("spotlights[");
  preamble += std::to_string(idx);
  preamble += "]";

  s->set_uniform(preamble + ".ambient", (Vector4f)(ambient_ * std::pow(2.0, intensity_)));
  s->set_uniform(preamble + ".diffuse", (Vector4f)(diffuse_ * std::pow(2.0, intensity_)));
  s->set_uniform(preamble + ".specular", (Vector4f)(specular_ * std::pow(2.0, intensity_)));

  s->set_uniform(preamble + ".position", pos_);
  s->set_uniform(preamble + ".direction", direction_);
  s->set_uniform(preamble + ".cutoff", std::cos(to_radians(cutoff_)));
  s->set_uniform(preamble + ".outer_cutoff", std::cos(to_radians(outer_cutoff_)));

  s->set_uniform(preamble + ".constant", constant_);
  s->set_uniform(preamble + ".linear", linear_);
  s->set_uniform(preamble + ".quadratic", quadratic_);
}
