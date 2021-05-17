#ifndef CANNON_GRAPHICS_GEOMETRY_SKYBOX_H
#define CANNON_GRAPHICS_GEOMETRY_SKYBOX_H 

#include <vector>
#include <string>

#include <cannon/graphics/cubemap.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/utils/class_forward.hpp>

namespace cannon {
  namespace graphics {

    CANNON_CLASS_FORWARD(ShaderProgram);
    CANNON_CLASS_FORWARD(VertexArrayObject);

    namespace geometry {

      class Skybox {
        public:
          Skybox() = delete;

          // Does not affect OpenGL state
          Skybox(std::vector<std::string> face_paths, const std::string&
              v_src="shaders/skybox.vert", const std::string&
              f_src="shaders/skybox.frag");

          // Does not affect OpenGL state
          void draw(const Matrix4f& view, const Matrix4f& perspective);

          ShaderProgramPtr program;

        private:
          void populate_bufs_();

          Cubemap cubemap_;

          VertexArrayObjectPtr vao_;
          VertexBuffer buf_;

          MatrixX3f vertices_;

      };

    } // namespace geometry
  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_GEOMETRY_SKYBOX_H */
