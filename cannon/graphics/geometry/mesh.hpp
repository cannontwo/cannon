#ifndef CANNON_GRAPHICS_GEOMETRY_MESH_H
#define CANNON_GRAPHICS_GEOMETRY_MESH_H 

#include <vector>
#include <memory>

#include <cannon/graphics/geometry/drawable_geom.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/element_buffer.hpp>
#include <cannon/graphics/texture.hpp>

namespace cannon {
  namespace graphics {
    namespace geometry {

      class Mesh : public DrawableGeom {
        public:
          Mesh() = delete;

          Mesh(std::shared_ptr<ShaderProgram> p, const MatrixX3f& vertices,
              const MatrixX3f& normals, const MatrixX2f& tex_coords,
              const MatrixX3u& indices,
              std::vector<std::shared_ptr<Texture>> diffuse_textures,
              std::vector<std::shared_ptr<Texture>> specular_textures) :
          vao_(new VertexArrayObject), buf_(vao_), normal_buf_(vao_),
          tex_coord_buf_(vao_), ebuf_(vao_), vertices_(vertices), normals_(normals),
          tex_coords_(tex_coords), indices_(indices), diffuse_textures_(diffuse_textures),
          specular_textures_(specular_textures) {

            assert(diffuse_textures_.size() <= max_diffuse_tex);
            assert(specular_textures_.size() <= max_specular_tex);

            program = p;
            populate_bufs_();
          }

          virtual ~Mesh() override {};

          virtual void draw(const Matrix4f& view, const Matrix4f& perspective) const override;
          virtual void draw(std::shared_ptr<ShaderProgram> p, const Matrix4f&
              view, const Matrix4f& perspective) const override;

          virtual Matrix4f get_model_mat() const override;

          void set_parent_model_mat(const MatrixX4f& mat);

        private:
          void populate_bufs_();

          std::shared_ptr<VertexArrayObject> vao_;
          VertexBuffer buf_;
          VertexBuffer normal_buf_;
          VertexBuffer tex_coord_buf_;
          ElementBuffer ebuf_;

          MatrixX3f vertices_;
          MatrixX3f normals_;
          MatrixX2f tex_coords_;
          MatrixX3u indices_;

          // Parent transform
          MatrixX4f parent_model_mat_;

          std::vector<std::shared_ptr<Texture>> diffuse_textures_;
          std::vector<std::shared_ptr<Texture>> specular_textures_;

          static const unsigned int max_diffuse_tex = 8;
          static const unsigned int max_specular_tex = 8;

          GLenum diffuse_gl_textures_[max_diffuse_tex] = {GL_TEXTURE0, 
                                                         GL_TEXTURE1,
                                                         GL_TEXTURE2,
                                                         GL_TEXTURE3,
                                                         GL_TEXTURE4,
                                                         GL_TEXTURE5,
                                                         GL_TEXTURE6,
                                                         GL_TEXTURE7};

          GLenum specular_gl_textures_[max_specular_tex] = {GL_TEXTURE8, 
                                                           GL_TEXTURE9,
                                                           GL_TEXTURE10,
                                                           GL_TEXTURE11,
                                                           GL_TEXTURE12,
                                                           GL_TEXTURE13,
                                                           GL_TEXTURE14,
                                                           GL_TEXTURE15};

      };

    } // namespace geometry
  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_GEOMETRY_MESH_H */
