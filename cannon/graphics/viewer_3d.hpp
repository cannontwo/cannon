#ifndef CANNON_GRAPHICS_VIEWER_3D
#define CANNON_GRAPHICS_VIEWER_3D 

#include <cmath>
#include <memory>
#include <random>

#include <cannon/graphics/camera.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/geometry/drawable_geom.hpp>
#include <cannon/graphics/projection.hpp>
#include <cannon/graphics/light.hpp>
#include <cannon/graphics/light_collection.hpp>
#include <cannon/graphics/geometry/cube.hpp>
#include <cannon/graphics/geometry/plane.hpp>
#include <cannon/graphics/geometry/textured_cube.hpp>
#include <cannon/graphics/geometry/model.hpp>
#include <cannon/graphics/point_light.hpp>
#include <cannon/graphics/spotlight.hpp>

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

          make_shaders_();
          initialize_lc_();
          set_callbacks_();

          auto plane = std::make_shared<geometry::Plane>(diffuse_program_);
          add_geom(plane);

          Vector3f pos;
          pos << 0.0, -5.0, 0.0;

          AngleAxisf rot(to_radians(90), Vector3f::UnitX());

          plane->set_pos(pos);
          plane->set_rot(rot);
          plane->set_scale(20.0);
        }

        ~Viewer3D() {}

        template <typename F>
        void render_loop(F f) {
          w.render_loop([&](){
            // Adjust camera movement speed based on render rate
            c.set_speed(2.5 * w.delta_time_);

            process_input_();

            Vector3f c_pos = c.get_pos();
            Vector4f tmp_pos;
            tmp_pos << c_pos[0],
                       c_pos[1],
                       c_pos[2],
                       1.0;
            diffuse_program_->set_uniform("viewPos", tmp_pos);
            light_program_->set_uniform("viewPos", tmp_pos);
            textured_program_->set_uniform("viewPos", tmp_pos);

            f();

            write_imgui();
            lc_.write_imgui();
            apply_light_collection(lc_);

            draw_scene_geom_();

          });
        }

        void add_geom(std::shared_ptr<geometry::DrawableGeom> g);
        void apply_light(std::shared_ptr<Light> l);
        void apply_light_collection(const LightCollection& l);

        std::shared_ptr<geometry::Cube> spawn_cube();
        std::shared_ptr<geometry::Plane> spawn_plane();
        std::shared_ptr<geometry::Model> spawn_model(const std::string& path);
        void spawn_point_light();
        void spawn_spotlight();

        void write_imgui();
        
        Window w;
        Camera c; 

      private:
        void make_shaders_();
        void initialize_lc_();
        void set_callbacks_();

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
        std::vector<std::shared_ptr<ShaderProgram>> shaders_;

        LightCollection lc_;

        std::shared_ptr<ShaderProgram> diffuse_program_;
        std::shared_ptr<ShaderProgram> light_program_;
        std::shared_ptr<ShaderProgram> textured_program_;
    };

    void drop_callback(GLFWwindow *window, int path_count, const char* paths[]);

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_VIEWER_3D */
