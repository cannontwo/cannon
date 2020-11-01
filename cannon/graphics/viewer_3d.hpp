#ifndef CANNON_GRAPHICS_VIEWER_3D
#define CANNON_GRAPHICS_VIEWER_3D 

#include <cmath>

#include <cannon/graphics/camera.hpp>
#include <cannon/graphics/window.hpp>

namespace cannon {
  namespace graphics {

    class Viewer3D {
      public:
        Viewer3D() : c({0.0, 0.0, 3.0}, {0.0, 0.0, -1.0}, {0.0, 1.0, 0.0}) {
          w.enable_depth_test();
          glfwSetInputMode(w.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
          });
        }
        
        Window w;
        Camera c; 

      private:
        void process_input_();

        double last_x_;
        double last_y_;
        double mouse_sensitivity_ = 0.005;

        bool first_mouse_ = true;
        float yaw_ = M_PI/2.0;
        float pitch_ = 0.0;
    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_VIEWER_3D */
