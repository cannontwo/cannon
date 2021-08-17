#ifndef CANNON_GRAPHICS_GEOMETRY_MESH_H
#define CANNON_GRAPHICS_GEOMETRY_MESH_H 

/*!
 * \file cannon/graphics/geometry/mesh.hpp
 * \brief File containing Mesh class definition.
 */

#include <vector>
#include <glad/glad.h>

#include <cannon/graphics/geometry/drawable_geom.hpp>

#include <cannon/utils/class_forward.hpp>

namespace cannon {
  namespace graphics {

    CANNON_CLASS_FORWARD(Texture);
    CANNON_CLASS_FORWARD(VertexBuffer);
    CANNON_CLASS_FORWARD(ElementBuffer);
    CANNON_CLASS_FORWARD(VertexArrayObject);

    using MatrixX3u = Matrix<unsigned int, Dynamic, 3>;
    using Matrix3Xu = Matrix<unsigned int, 3, Dynamic>;

    namespace geometry {

      class Mesh : public DrawableGeom {
        public:
          Mesh() = delete;

          Mesh(std::shared_ptr<ShaderProgram> p, const MatrixX3f& vertices,
              const MatrixX3f& normals, const MatrixX2f& tex_coords, const
              MatrixX3u& indices, Material material,
              std::vector<TexturePtr> diffuse_textures,
              std::vector<TexturePtr> specular_textures);

          virtual ~Mesh() override {};

          virtual void draw(const Matrix4f& view, const Matrix4f& perspective) const override;
          virtual void draw(std::shared_ptr<ShaderProgram> p, const Matrix4f&
              view, const Matrix4f& perspective) const override;

          virtual Matrix4f get_model_mat() const override;

          void set_parent_model_mat(const MatrixX4f& mat);

        private:
          void populate_bufs_();

          VertexArrayObjectPtr vao_;
          VertexBufferPtr buf_;
          VertexBufferPtr normal_buf_;
          VertexBufferPtr tex_coord_buf_;
          ElementBufferPtr ebuf_;

          MatrixX3f vertices_;
          MatrixX3f normals_;
          MatrixX2f tex_coords_;
          MatrixX3u indices_;

          // Parent transform
          MatrixX4f parent_model_mat_;

          std::vector<TexturePtr> diffuse_textures_;
          std::vector<TexturePtr> specular_textures_;

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
