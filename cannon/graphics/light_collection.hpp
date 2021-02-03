#ifndef CANNON_GRAPHICS_LIGHT_COLLECTION
#define CANNON_GRAPHICS_LIGHT_COLLECTION 

/*!
 * \file cannon/graphics/light_collection.hpp
 * File containing LightCollection class definition.
 */

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
    
    /*!
     * \brief Class representing a collection of lights in a scene.
     *
     * This collection consists of point lights, spotlights, and a single
     * directional light. Keeping all lights together in one object makes it
     * easy to interface with shaders expecting multiple lights.
     */
    class LightCollection {
      public:

        /*!
         * Method to apply this light collection on geometry. Essentially just
         * sets uniforms on the geometry's shader.
         *
         * \param g The geometry to apply this light to.
         */
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

        /*!
         * Method to apply this light collection to a shader. Essentially just
         * sets uniforms corresponding to the lights in this collection.
         *
         * \param s The shader to apply this collection to.
         */
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

        /*!
         * Method to add a point light to this collection.
         *
         * \param l The point light to add.
         */
        void add_light(std::shared_ptr<PointLight> l) {
          if (point_lights_.size() == max_points_) {
            point_lights_.pop_front();
          }

          point_lights_.push_back(l);
        }

        /*!
         * Method to add a spotlight to this collection.
         *
         * \param l The spotlight to add.
         */
        void add_light(std::shared_ptr<Spotlight> l) {
          if (spotlights_.size() == max_spots_) {
            spotlights_.pop_front();
          }

          spotlights_.push_back(l);
        }

        /*!
         * Method to set the directional light for this collection.
         *
         * \param l The new directional light.
         */
        void add_light(std::shared_ptr<DirectionalLight> l) {
          directional_light_ = l;
        }

        /*!
         * Method to get the directional light for this collection.
         *
         * \returns A pointer to the directional light.
         */
        std::shared_ptr<DirectionalLight> get_directional_light() {
          return directional_light_;
        }

        /*!
         * Method to write imgui controls for this light collection, delegating
         * to each contained light.
         */
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
        unsigned int max_points_ = 16; //!< Maximum number of point lights in this collection.
        unsigned int max_spots_ = 16; //!< Maximum number of spotlights in this collection.

        std::deque<std::shared_ptr<PointLight>> point_lights_; //!< Point lights in this collection.
        std::deque<std::shared_ptr<Spotlight>> spotlights_; //!< Spotlights in this collection.
        std::shared_ptr<DirectionalLight> directional_light_; //!< Directional light.

    }; 

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_LIGHT_COLLECTION */
