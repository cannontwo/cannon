#pragma once
#ifndef CANNON_GRAPHICS_VIEWER_3D
#define CANNON_GRAPHICS_VIEWER_3D 

/*!
 * \file cannon/graphics/viewer_3d.hpp
 * \brief File containing Viewer3D class definition and callback functions.
 */

#include <cmath>
#include <memory>
#include <random>
#include <queue>
#include <functional>

#include <cannon/graphics/camera.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/light_collection.hpp>

#include <cannon/utils/class_forward.hpp>

namespace cannon {
  namespace graphics {

    namespace geometry {
      CANNON_CLASS_FORWARD(DrawableGeom);
      CANNON_CLASS_FORWARD(Cube);
      CANNON_CLASS_FORWARD(Plane);
      CANNON_CLASS_FORWARD(Model);
      CANNON_CLASS_FORWARD(Skybox);
    }

    CANNON_CLASS_FORWARD(ShaderProgram);
    CANNON_CLASS_FORWARD(Light);
    CANNON_CLASS_FORWARD(RenderPass);
    CANNON_CLASS_FORWARD(Texture);

    /*!
     * \brief Class representing an OpenGL context in which objects will be
     * displayed in 3D. Allows for spawning objects and interactive
     * manipulation of the camera.
     */
    class Viewer3D {
      public:

        /*!
         * \brief Default constructor.
         */
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

        /*!
         * \brief Destructor.
         */
        ~Viewer3D() {}

        /*!
         * \brief Method that runs the rendering loop for this window. On each
         * loop, the input function is run in addition to this class's
         * rendering behavior.
         *
         * \param f Function to run on each frame. 
         * \param clear Whether to clear the screen after each frame.
         */
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

        /*!
         * \brief Method that runs the rendering loop for this window in a
         * multipass configuration. On each loop, the input function is run
         * after all rendering passes for this object.
         *
         * \param f Function to run on each frame.
         * \param clear Whether to clear the screen after each frame.
         */
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

        /*!
         * \brief Method to draw all render passes managed by this viewer.
         */
        void draw();

        /*!
         * \brief Method to add the input geometry to the scene visualized by this object.
         *
         * \param g The geometry to add.
         */
        void add_geom(std::shared_ptr<geometry::DrawableGeom> g);

        /*!
         * \brief Method to add a shader to the collection managed by this
         * object. This allows for, for example, applying the scene's lights to
         * all geometry shaders.
         *
         * \param s The shader to add.
         */
        void add_shader(ShaderProgramPtr s);
        
        /*!
         * \brief Method to apply a light to all geometry in this scene.
         *
         * \param l The light to apply.
         */
        void apply_light(LightPtr l);

        /*!
         * \brief Method to apply a light collection to all geometry in this scene.
         *
         * \param l The light collection to apply.
         */
        void apply_light_collection(const LightCollection& l);

        /*!
         * \brief Method to apply the light collection managed by this object to the input shader.
         *
         * \param p The shader to apply the light collection to.
         */
        void apply_light_collection(ShaderProgramPtr p);

        /*!
         * \brief Set the skybox for this viewer.
         *
         * \param face_paths Paths for six faces to load as a skybox.
         */
        void set_skybox(std::vector<std::string> face_paths);

        /*!
         * \brief Enable skybox drawing.
         */
        void enable_skybox();

        /*!
         * \brief Disable skybox drawing.
         */
        void disable_skybox();

        void draw_scene_geom(bool draw_lights = true, bool ortho = false);
        void draw_scene_geom(ShaderProgramPtr p, bool draw_lights = true, 
            bool ortho = false);

        void draw_light_geom();
        void draw_light_geom(ShaderProgramPtr p);

        void draw_sdf_geom(bool ortho = false);
        void draw_sdf_geom(ShaderProgramPtr p, bool ortho = false);

        Vector3f get_directional_light_pos();
        Vector3f get_directional_light_dir();

        void add_render_pass(RenderPassPtr rp);
        std::shared_ptr<Framebuffer> add_render_pass(const std::string& name,
            ShaderProgramPtr p, std::function<void()> f);
        std::shared_ptr<Framebuffer> add_render_pass(const std::string& name,
            std::vector<TexturePtr> attachments,
            std::vector<ShaderProgramPtr> programs, std::function<void()> f);

        geometry::CubePtr spawn_cube();
        geometry::PlanePtr spawn_plane();
        geometry::ModelPtr spawn_model(const std::string& path);
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

        std::vector<geometry::DrawableGeomPtr> scene_geom_;
        std::vector<geometry::DrawableGeomPtr> light_geom_;
        std::vector<geometry::DrawableGeomPtr> sdf_geom_;
        std::vector<ShaderProgramPtr> shaders_;
        std::deque<RenderPassPtr> render_passes_;

        LightCollection lc_;

        ShaderProgramPtr geom_program_;
        ShaderProgramPtr sdf_program_;

        bool draw_skybox_ = false;
        geometry::SkyboxPtr skybox_;
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
