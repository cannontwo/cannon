#include <cannon/graphics/geometry/mesh.hpp>

#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/texture.hpp>
#include <cannon/graphics/vertex_array_object.hpp>

using namespace cannon::graphics::geometry;

Mesh::Mesh(std::shared_ptr<ShaderProgram> p, const MatrixX3f& vertices, const
    MatrixX3f& normals, const MatrixX2f& tex_coords, const MatrixX3u& indices,
    Material material, std::vector<std::shared_ptr<Texture>> diffuse_textures,
    std::vector<std::shared_ptr<Texture>> specular_textures) : vao_(new
      VertexArrayObject), buf_(vao_), normal_buf_(vao_), tex_coord_buf_(vao_),
    ebuf_(vao_), vertices_(vertices), normals_(normals),
    tex_coords_(tex_coords), indices_(indices),
    diffuse_textures_(diffuse_textures), specular_textures_(specular_textures)
{

  assert(diffuse_textures_.size() <= max_diffuse_tex);
  assert(specular_textures_.size() <= max_specular_tex);

  material_ = material;

  program = p; populate_bufs_();

  name_ = "Mesh"; }

void Mesh::draw(const Matrix4f& view, const Matrix4f& perspective) const {
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

  program->activate();
  
  buf_.bind();
  normal_buf_.bind();
  tex_coord_buf_.bind();
  ebuf_.bind();

  glDrawElements(GL_TRIANGLES, indices_.rows() * indices_.cols(), GL_UNSIGNED_INT, 0);

  program->deactivate();

  ebuf_.unbind();
  tex_coord_buf_.unbind();
  normal_buf_.unbind();
  buf_.unbind();

  for (unsigned int i = 0; i < diffuse_textures_.size(); i++) {
    diffuse_textures_[i]->unbind(diffuse_gl_textures_[i]);
  }
  for (unsigned int i = 0; i < specular_textures_.size(); i++) {
    specular_textures_[i]->unbind(specular_gl_textures_[i]);
  }

  glActiveTexture(GL_TEXTURE0);
}

void Mesh::draw(std::shared_ptr<ShaderProgram> p, const Matrix4f& view, const
    Matrix4f& perspective) const {
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
  
  p->activate();
  
  buf_.bind();
  normal_buf_.bind();
  tex_coord_buf_.bind();
  ebuf_.bind();

  glDrawElements(GL_TRIANGLES, indices_.rows() * indices_.cols(), GL_UNSIGNED_INT, 0);

  program->deactivate();

  ebuf_.unbind();
  tex_coord_buf_.unbind();
  normal_buf_.unbind();
  buf_.unbind();

  for (unsigned int i = 0; i < diffuse_textures_.size(); i++) {
    diffuse_textures_[i]->unbind(diffuse_gl_textures_[i]);
  }
  for (unsigned int i = 0; i < specular_textures_.size(); i++) {
    specular_textures_[i]->unbind(specular_gl_textures_[i]);
  }

  glActiveTexture(GL_TEXTURE0);
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
