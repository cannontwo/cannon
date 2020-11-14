#include <cannon/graphics/geometry/mesh.hpp>

using namespace cannon::graphics::geometry;

void Mesh::draw(const Matrix4f& view, const Matrix4f& perspective) const {
  program->activate();
  program->set_uniform("model", get_model_mat());
  program->set_uniform("view", view);
  program->set_uniform("projection", perspective);
  program->set_uniform("normalmat", get_normal_mat(), false);
  program->set_uniform("material.ambient", material_.ambient);
  program->set_uniform("material.diffuse", material_.diffuse);
  program->set_uniform("material.specular", material_.specular);
  program->set_uniform("material.shininess", material_.shininess);

  for (unsigned int i = 0; i < diffuse_textures_.size(); i++) {
    program->set_uniform(std::string("material.diffuse_tex[") + std::to_string(i) + "]", 
        (int)i);
    diffuse_textures_[i]->bind(diffuse_gl_textures_[i]);
  }
  for (unsigned int i = 0; i < specular_textures_.size(); i++) {
    program->set_uniform(std::string("material.specular_tex[") + std::to_string(i) + "]", 
        (int)(max_diffuse_tex + i));
    specular_textures_[i]->bind(specular_gl_textures_[i]);
  }
  
  buf_.bind();
  normal_buf_.bind();
  tex_coord_buf_.bind();
  ebuf_.bind();

  glDrawElements(GL_TRIANGLES, indices_.rows() * indices_.cols(), GL_UNSIGNED_INT, 0);
}

void Mesh::draw(std::shared_ptr<ShaderProgram> p, const Matrix4f& view, const
    Matrix4f& perspective) const {
  p->activate();
  p->set_uniform("model", get_model_mat());
  p->set_uniform("view", view);
  p->set_uniform("projection", perspective);
  p->set_uniform("normalmat", get_normal_mat(), false);
  p->set_uniform("material.ambient", material_.ambient);
  p->set_uniform("material.diffuse", material_.diffuse);
  p->set_uniform("material.specular", material_.specular);
  p->set_uniform("material.shininess", material_.shininess);

  for (unsigned int i = 0; i < diffuse_textures_.size(); i++) {
    p->set_uniform(std::string("material.diffuse_tex[") + std::to_string(i) + "]", 
        (int)i);
    diffuse_textures_[i]->bind(diffuse_gl_textures_[i]);
  }
  for (unsigned int i = 0; i < specular_textures_.size(); i++) {
    p->set_uniform(std::string("material.specular_tex[") + std::to_string(i) + "]", 
        (int)(max_diffuse_tex + i));
    specular_textures_[i]->bind(specular_gl_textures_[i]);
  }
  
  buf_.bind();
  normal_buf_.bind();
  tex_coord_buf_.bind();
  ebuf_.bind();

  glDrawElements(GL_TRIANGLES, indices_.rows() * indices_.cols(), GL_UNSIGNED_INT, 0);

}

Matrix4f Mesh::get_model_mat() const {
  Affine3f trans;
  trans = AngleAxisf(to_radians(0.0f), Vector3f::UnitX()) *
    Translation3f(pos_) * rot_ * Scaling(scale_);

  return parent_model_mat_ * trans.matrix();
}

void Mesh::set_parent_model_mat(const MatrixX4f& mat) {
  parent_model_mat_ = mat;
}

void Mesh::populate_bufs_() {
  assert(vertices_.rows() == normals_.rows());
  assert(vertices_.rows() == tex_coords_.rows());

  buf_.buffer(vertices_);
  normal_buf_.buffer(normals_);
  tex_coord_buf_.buffer(tex_coords_);
  ebuf_.buffer(indices_);
}
