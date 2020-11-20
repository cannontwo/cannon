#ifndef CANNON_GRAPHICS_LIGHT_COLLECTION
#define CANNON_GRAPHICS_LIGHT_COLLECTION 

#include <queue>
#include <memory>

#include <cannon/graphics/point_light.hpp>
#include <cannon/graphics/spotlight.hpp>
#include <cannon/graphics/directional_light.hpp>
#include <cannon/graphics/geometry/drawable_geom.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::log;

namespace cannon {
  namespace graphics {
    
    class LightCollection {
      public:

        void apply(std::shared_ptr<geometry::DrawableGeom> g) const {
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

        void apply(std::shared_ptr<ShaderProgram> s) const {
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

        void add_light(std::shared_ptr<PointLight> l) {
          if (point_lights_.size() == max_points_) {
            point_lights_.pop_front();
          }

          point_lights_.push_back(l);
        }

        void add_light(std::shared_ptr<Spotlight> l) {
          if (spotlights_.size() == max_spots_) {
            spotlights_.pop_front();
          }

          spotlights_.push_back(l);
        }

        void add_light(std::shared_ptr<DirectionalLight> l) {
          directional_light_ = l;
        }

        std::shared_ptr<DirectionalLight> get_directional_light() {
          return directional_light_;
        }

        virtual void write_imgui() {
          if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("Lighting")) {
              for (unsigned int i = 0; i < point_lights_.size(); i++) {
                point_lights_[i]->write_imgui(i);
              }

              for (unsigned int i = 0; i < spotlights_.size(); i++) {
                spotlights_[i]->write_imgui(i);
              }

              if (directional_light_)
                directional_light_->write_imgui(0);

              ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
          }
        }

      private:
        unsigned int max_points_ = 16;
        unsigned int max_spots_ = 16;

        std::deque<std::shared_ptr<PointLight>> point_lights_;
        std::deque<std::shared_ptr<Spotlight>> spotlights_;
        std::shared_ptr<DirectionalLight> directional_light_;

    }; 

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_LIGHT_COLLECTION */
