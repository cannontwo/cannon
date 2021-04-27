#pragma once
#ifndef CANNON_GRAPHICS_VIEWER_3D
#define CANNON_GRAPHICS_VIEWER_3D 

#include <cmath>
#include <memory>
#include <random>
#include <queue>
#include <functional>

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
#include <cannon/graphics/geometry/skybox.hpp>
#include <cannon/graphics/render_pass.hpp>
#include <cannon/graphics/geometry/sdf_volume.hpp>

namespace cannon {
  namespace graphics {

    class Viewer3D {
      public:
        Viewer3D() : c({0.0, 0.0, 3.0}, {0.0, 0.0, -1.0}, {0.0, 1.0, 0.0}) {
          w.enable_depth_test();
          w.enable_face_culling();

          glEnable(GL_FRAMEBUFFER_SRGB); 

          last_x_ = w.width / 2;
          last_y_ = w.height / 2;

          make_shaders_();
          initialize_lc_();
          set_callbacks_();
          populate_initial_geometry_();
        }

        ~Viewer3D() {}

        template <typename F>
        void render_loop(F f, bool clear = true) {
          w.render_loop([&](){
            // Adjust camera movement speed based on render rate
            //c.set_speed(1e5 * w.delta_time_);
            c.update_pos();

            write_imgui();
            lc_.write_imgui();

            Vector3f c_pos = c.get_pos();
            Vector4f tmp_pos;
            tmp_pos << c_pos[0],
                       c_pos[1],
                       c_pos[2],
                       1.0;
            geom_program_->set_uniform("viewPos", tmp_pos);
            //apply_light_collection(lc_);
            lc_.apply(geom_program_);
            draw_scene_geom(geom_program_);

            f();

          }, clear);
        }

        template <typename F>
        void render_loop_multipass(F f, bool clear = true) {
          w.render_loop([&](){
            // Adjust camera movement speed based on render rate
            //c.set_speed(1e5 * w.delta_time_);
            c.update_pos();

            draw();

            f();

          }, clear);

        }

        void draw();

        void add_geom(std::shared_ptr<geometry::DrawableGeom> g);
        void add_shader(std::shared_ptr<ShaderProgram> s);
        void apply_light(std::shared_ptr<Light> l);
        void apply_light_collection(const LightCollection& l);
        void apply_light_collection(std::shared_ptr<ShaderProgram> p);

        void set_skybox(std::vector<std::string> face_paths);
        void enable_skybox();
        void disable_skybox();

        void draw_scene_geom(bool draw_lights = true, bool ortho = false);
        void draw_scene_geom(std::shared_ptr<ShaderProgram> p, bool draw_lights = true, 
            bool ortho = false);

        void draw_light_geom();
        void draw_light_geom(std::shared_ptr<ShaderProgram> p);

        void draw_sdf_geom(bool ortho = false);
        void draw_sdf_geom(std::shared_ptr<ShaderProgram> p, bool ortho = false);

        Vector3f get_directional_light_pos();
        Vector3f get_directional_light_dir();

        void add_render_pass(std::shared_ptr<RenderPass> rp);
        std::shared_ptr<Framebuffer> add_render_pass(const std::string& name,
            std::shared_ptr<ShaderProgram> p, std::function<void()> f);
        std::shared_ptr<Framebuffer> add_render_pass(const std::string& name,
            std::vector<std::shared_ptr<Texture>> attachments,
            std::vector<std::shared_ptr<ShaderProgram>> programs, std::function<void()> f);

        std::shared_ptr<geometry::Cube> spawn_cube();
        std::shared_ptr<geometry::Plane> spawn_plane();
        std::shared_ptr<geometry::Model> spawn_model(const std::string& path);
        void spawn_point_light();
        void spawn_spotlight();
        void spawn_sdf_volume();

        void write_imgui(bool multipass=false);

        ImageData get_image(const std::string &path = "") const;

        void close() const;
        
        Window w;
        Camera c; 

        friend void viewer3d_framebuffer_size_callback(GLFWwindow* window, int width, int height);
        friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        friend void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

      private:
        void make_shaders_();
        void initialize_lc_();
        void set_callbacks_();
        void populate_initial_geometry_();

        double last_x_;
        double last_y_;
        double mouse_sensitivity_ = 0.005;

        bool first_mouse_ = true;
        bool mouse_captured_ = false;
        double last_capture_time_;
        float yaw_ = M_PI/2.0;
        float pitch_ = 0.0;

        std::vector<std::shared_ptr<geometry::DrawableGeom>> scene_geom_;
        std::vector<std::shared_ptr<geometry::DrawableGeom>> light_geom_;
        std::vector<std::shared_ptr<geometry::DrawableGeom>> sdf_geom_;
        std::vector<std::shared_ptr<ShaderProgram>> shaders_;
        std::deque<std::shared_ptr<RenderPass>> render_passes_;

        LightCollection lc_;

        std::shared_ptr<ShaderProgram> geom_program_;
        std::shared_ptr<ShaderProgram> sdf_program_;

        bool draw_skybox_ = false;
        std::shared_ptr<geometry::Skybox> skybox_;
    };

    void drop_callback(GLFWwindow *window, int path_count, const char* paths[]);

    // Necessary since we change the user window pointer
    void viewer3d_framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void viewer_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_VIEWER_3D */
