#ifndef CANNON_GRAPHICS_GEOMETRY_SKYBOX_H
#define CANNON_GRAPHICS_GEOMETRY_SKYBOX_H 

#include <vector>
#include <string>
#include <memory>

#include <cannon/graphics/cubemap.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/vertex_buffer.hpp>

namespace cannon {
  namespace graphics {
    namespace geometry {

      class Skybox {
        public:
          Skybox() = delete;

          Skybox(std::vector<std::string> face_paths, const std::string&
              v_src="shaders/skybox.vert", const std::string&
              f_src="shaders/skybox.frag") : cubemap_(face_paths), 
            vao_(new VertexArrayObject), buf_(vao_), vertices_(36, 3) {

            program = std::make_shared<ShaderProgram>("skybox_shader");
            program->attach_vertex_shader(v_src);
            program->attach_fragment_shader(f_src);
            program->link();

            populate_bufs_();

          }

          void draw(const Matrix4f& view, const Matrix4f& perspective);

          std::shared_ptr<ShaderProgram> program;

        private:
          void populate_bufs_();

          Cubemap cubemap_;

          std::shared_ptr<VertexArrayObject> vao_;
          VertexBuffer buf_;

          MatrixX3f vertices_;

      };

    } // namespace geometry
  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_GEOMETRY_SKYBOX_H */
