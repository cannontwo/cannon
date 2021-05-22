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

        /*!
         * \brief Draw all of the regular geometry in the scene handled by this viewer. 
         *
         * \param draw_lights Whether to draw light geometry.
         * \param ortho Whether to use orthographic projection, rather than perspective projection.
         */
        void draw_scene_geom(bool draw_lights = true, bool ortho = false);

        /*!
         * \brief Draw all of the regular geometyr in the scene handled by this viewer using the input shader.
         *
         * \param p The shader to use for drawing
         * \param draw_lights Whether to draw light geometry.
         * \param ortho Whether to use orthographic projection, rather than perspective projection.
         */
        void draw_scene_geom(ShaderProgramPtr p, bool draw_lights = true, 
            bool ortho = false);

        /*!
         * \brief Draw geometry representing all lights in the scene handled by this viewer.
         */
        void draw_light_geom();

        /*!
         * \brief Draw geometry representing all lights in the scene handled by
         * this viewer using the input shader.
         *
         * \param p The shader to use for drawing light geometry.
         */
        void draw_light_geom(ShaderProgramPtr p);

        /*!
         * \brief Draw SDF geometry in the scene.
         *
         * \param ortho Whether to use orthographic projection, rather than perspective projection.
         */
        void draw_sdf_geom(bool ortho = false);
        
        /*!
         * \brief Draw SDF geometyr in the scene using the input shader program.
         * 
         * \param p The shader to use for drawing.
         * \param ortho Whether to use orthographic projection, rather than perspective projection.
         */
        void draw_sdf_geom(ShaderProgramPtr p, bool ortho = false);

        /*!
         * \brief Get the position of the assumed one directional light in this scene.
         *
         * \returns Position for the assumed directional light.
         */
        Vector3f get_directional_light_pos();

        /*!
         * \brief Get the direction of the assumed one directional light in this scene.
         *
         * \returns A vector representing the directional light direction.
         */
        Vector3f get_directional_light_dir();

        /*!
         * \brief Add a render pass to this viewer.
         *
         * \param rp The render pass to add. 
         */
        void add_render_pass(RenderPassPtr rp);

        /*!
         * \brief Construct and add a render pass to this viewer from its name, shader, and render function.
         *
         * \param name Name for the renderpass.
         * \param p Shader program for the renderpass.
         * \param f Rendering function to be called each frame for the renderpass.
         *
         * \returns A pointer to the framebuffer for the constructed renderpass.
         */
        std::shared_ptr<Framebuffer> add_render_pass(const std::string& name,
            ShaderProgramPtr p, std::function<void()> f);

        /*!
         * \brief Construct and add a render pass to this viewer from its name,
         * color attachments, shader, and render function.
         *
         * \param name Name for the renderpass.
         * \param attachments Color attachments for the constructed render pass.
         * \param p Shader program for the renderpass.
         * \param f Rendering function to be called each frame for the renderpass.
         *
         * \returns A pointer to the framebuffer for the constructed renderpass.
         */
        std::shared_ptr<Framebuffer> add_render_pass(const std::string& name,
            std::vector<TexturePtr> attachments,
            std::vector<ShaderProgramPtr> programs, std::function<void()> f);

        /*!
         * \brief Spawn a cube in front of the current camera position.
         *
         * \returns A pointer to the spawned cube.
         */
        geometry::CubePtr spawn_cube();

        /*!
         * \brief Spawn a plane in front of the current camera position.
         *
         * \returns A pointer to the spawned plane.
         */
        geometry::PlanePtr spawn_plane();

        /*!
         * \brief Loads a model from the input path, then spawns it in front of the current camera position.
         *
         * \param path The model path to load.
         * \returns A pointer to the spawned model.
         */
        geometry::ModelPtr spawn_model(const std::string& path);

        /*!
         * \brief Spawn a point light in front of the current camera position.
         *
         * \returns A pointer to the spawned light.
         */
        void spawn_point_light();

        /*!
         * \brief Spawn a spotlight in front of the current camera position.
         *
         * \returns A pointer to the spawned light.
         */
        void spawn_spotlight();

        /*!
         * \brief Spawn an SDF volume in front of the current camera position.
         *
         * \returns A pointer to the spawned SDF volume.
         */
        void spawn_sdf_volume();

        /*!
         * \brief Write ImGui controls for this viewer.
         *
         * \param multipass Whether to write controls for renderpasses
         * controlled by this viewer.
         */
        void write_imgui(bool multipass=false);

        /*!
         * \brief Get an image of the current OpenGL rendered image, annotated
         * with a path to save it to.
         *
         * \path The path to save this image to. Will be stored in the returned
         * ImageData.
         *
         * \returns An object representing the image data for further handling.
         */
        ImageData get_image(const std::string &path = "") const;

        /*!
         * \brief Close this viewer.
         */
        void close() const;
        
        Window w; //!< Internal window object encapsulating OpenGL state.
        Camera c; //!< The camera for this viewer.

        friend void viewer3d_framebuffer_size_callback(GLFWwindow* window, int width, int height);
        friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        friend void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

      private:
        
        /*!
         * \brief Construct default shaders for this viewer.
         */
        void make_shaders_();
        
        /*!
         * \brief Initialize light collection for this viewer with a directional
         * light.
         */
        void initialize_lc_();

        /*!
         * \brief Setup OpenGL callbacks for this viewer.
         */
        void set_callbacks_();

        /*!
         * \brief Spawn initial scene geometry.
         */
        void populate_initial_geometry_();

        double last_x_; //!< Last mouse x position
        double last_y_; //!< Last mouse y position
        double mouse_sensitivity_ = 0.005; //!< Mouse sensitivity modifier

        bool first_mouse_ = true; //!< Whether the mouse is moving in the OpenGL context for the first time
        bool mouse_captured_ = false; //!< Whether this window captures the mouse
        double last_capture_time_; //!< Last time the mouse was captured
        float yaw_ = M_PI/2.0; //!< Camera yaw
        float pitch_ = 0.0; //!< Camera pitch

        std::vector<geometry::DrawableGeomPtr> scene_geom_; //!< Scene regular geometry
        std::vector<geometry::DrawableGeomPtr> light_geom_; //!< Scene light geometry
        std::vector<geometry::DrawableGeomPtr> sdf_geom_; //!< Scene SDF geometry
        std::vector<ShaderProgramPtr> shaders_; //!< Shaders for this viewer
        std::deque<RenderPassPtr> render_passes_; //!< Render passes for this viewer

        LightCollection lc_; //!< Light collection for this viewer

        ShaderProgramPtr geom_program_; //!< Default geometry shader
        ShaderProgramPtr sdf_program_; //!< Default SDF shader

        bool draw_skybox_ = false; //!< Whether to draw a skybox for this viewer
        geometry::SkyboxPtr skybox_; //!< Skybox for this viewer
    };

    /*!
     * \brief Callback for mouse drag-drop callbacks on Viewer3D. Loads a
     * model from the dropped path and spawns it.
     *
     * \param window OpenGL context
     * \param path_count Number of dropped paths
     * \param paths Paths dropped on the window
     */
    void drop_callback(GLFWwindow *window, int path_count, const char* paths[]);

    /*!
     * \brief Resize callback for Viewer3D.
     *
     * \param window OpenGL context
     * \param width New width of the window.
     * \param height New height of the window.
     */
    void viewer3d_framebuffer_size_callback(GLFWwindow* window, int width, int height);
    
    /*!
     * \brief Key press callback for Viewer3D.
     *
     * \param window OpenGL context
     * \param key Key pressed
     * \param scancode OS-specific key representation
     * \param action Kind of key press event
     * \param mods Modifying keys held down
     */
    void viewer_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    /*!
     * \brief Mouse button callback for Viewer3D.
     *
     * \param window OpenGL context
     * \param button Mouse button pressed
     * \param action Kind of key press event
     * \param mods Modifying keys held down
     */
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

    /*!
     * \brief Cursor position callback for Viewer3D.
     *
     * \param window OpenGL context
     * \param xpos New cursor x-coordinate
     * \param xpos New cursor y-coordinate
     */
    void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_VIEWER_3D */
