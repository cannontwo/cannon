#ifndef CANNON_GRAPHICS_VIEWER_3D
#define CANNON_GRAPHICS_VIEWER_3D 

#include <cmath>
#include <memory>

#include <cannon/graphics/camera.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/geometry/drawable_geom.hpp>
#include <cannon/graphics/projection.hpp>
#include <cannon/graphics/light.hpp>
#include <cannon/graphics/light_collection.hpp>

namespace cannon {
  namespace graphics {

    class Viewer3D {
      public:
        Viewer3D() : c({0.0, 0.0, 3.0}, {0.0, 0.0, -1.0}, {0.0, 1.0, 0.0}) {
          w.enable_depth_test();

          glEnable(GL_FRAMEBUFFER_SRGB); 
          glfwSetInputMode(w.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
          last_capture_time_ = glfwGetTime();

          last_x_ = w.width / 2;
          last_y_ = w.height / 2;
        }

        template <typename F>
        void render_loop(F f) {
          w.render_loop([&](){
            // Adjust camera movement speed based on render rate
            c.set_speed(2.5 * w.delta_time_);

            process_input_();

            f();

            draw_scene_geom_();
          });
        }

        void add_geom(std::shared_ptr<geometry::DrawableGeom> g);
        void apply_light(std::shared_ptr<Light> l);
        void apply_light_collection(const LightCollection& l);
        
        Window w;
        Camera c; 

      private:
        void process_input_();
        void process_mouse_input_();
        void draw_scene_geom_();

        double last_x_;
        double last_y_;
        double mouse_sensitivity_ = 0.005;

        bool first_mouse_ = true;
        bool mouse_captured_ = true;
        double last_capture_time_;
        float yaw_ = M_PI/2.0;
        float pitch_ = 0.0;

        std::vector<std::shared_ptr<geometry::DrawableGeom>> scene_geom_;
    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_VIEWER_3D */
