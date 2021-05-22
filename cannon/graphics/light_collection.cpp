#include <cannon/graphics/light_collection.hpp>

#include <cannon/graphics/shader_program.hpp>

using namespace cannon::graphics;

void LightCollection::apply(std::shared_ptr<geometry::DrawableGeom> g) const {
  g->program->set_uniform("num_point_lights", (int)point_lights_.size());
  g->program->set_uniform("num_spotlights", (int)spotlights_.size());

  for (unsigned int i = 0; i < point_lights_.size(); i++) {
    point_lights_[i]->apply(g, i);
  }

  for (unsigned int i = 0; i < spotlights_.size(); i++) {
    spotlights_[i]->apply(g, i);
  }

  if (directional_light_)
    directional_light_->apply(g);
}

void LightCollection::apply(std::shared_ptr<ShaderProgram> s) const {
  s->set_uniform("num_point_lights", (int)point_lights_.size());
  s->set_uniform("num_spotlights", (int)spotlights_.size());

  for (unsigned int i = 0; i < point_lights_.size(); i++) {
    point_lights_[i]->apply(s, i);
  }

  for (unsigned int i = 0; i < spotlights_.size(); i++) {
    spotlights_[i]->apply(s, i);
  }

  if (directional_light_)
    directional_light_->apply(s);
}
