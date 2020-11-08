#include <cannon/graphics/geometry/mesh.hpp>

using namespace cannon::graphics::geometry;

void Mesh::draw(const Matrix4f& view, const Matrix4f& perspective) const {
  program->activate();
  program->set_uniform("model", get_model_mat());
  program->set_uniform("view", view);
  program->set_uniform("projection", perspective);
  program->set_uniform("normalmat", get_normal_mat(), false);

  for (unsigned int i = 0; i < diffuse_textures_.size(); i++) {
    program->set_uniform(std::string("material.diffuse[") + std::to_string(i) + "]", 
        (int)i);
    diffuse_textures_[i]->bind(diffuse_gl_textures_[i]);
  }
  for (unsigned int i = 0; i < specular_textures_.size(); i++) {
    program->set_uniform(std::string("material.diffuse[") + std::to_string(i) + "]", 
        (int)(max_diffuse_tex + i));
    specular_textures_[i]->bind(specular_gl_textures_[i]);
  }
  
  buf_.bind();
  normal_buf_.bind();
  tex_coord_buf_.bind();
  ebuf_.bind();

  glDrawElements(GL_TRIANGLES, indices_.rows() * indices_.cols(), GL_UNSIGNED_INT, 0);
}

void Mesh::populate_bufs_() {
  assert(vertices_.rows() == normals_.rows());
  assert(vertices_.rows() == tex_coords_.rows());

  buf_.buffer(vertices_);
  normal_buf_.buffer(normals_);
  tex_coord_buf_.buffer(tex_coords_);
  ebuf_.buffer(indices_);
}
