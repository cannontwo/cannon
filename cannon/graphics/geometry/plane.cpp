#include <cannon/graphics/geometry/plane.hpp>

using namespace cannon::graphics::geometry;

void Plane::draw(const Matrix4f& view, const Matrix4f& perspective) const {
  program->set_uniform("model", get_model_mat());
  program->set_uniform("view", view);
  program->set_uniform("projection", perspective);
  program->set_uniform("normalmat", get_normal_mat(), false);
  
  // Material properties
  program->set_uniform("material.ambient", material_.ambient);
  program->set_uniform("material.diffuse", material_.diffuse);
  program->set_uniform("material.specular", material_.specular);
  program->set_uniform("material.shininess", material_.shininess);

  program->activate();

  buf_.bind();
  normal_buf_.bind();

  glBindTexture(GL_TEXTURE_2D, 0);
  glDrawArrays(GL_TRIANGLES, 0, vertices_.rows());

  program->deactivate();

  normal_buf_.unbind();
  buf_.unbind();
}

void Plane::draw(std::shared_ptr<ShaderProgram> p, const Matrix4f& view, const
    Matrix4f& perspective) const {
  p->set_uniform("model", get_model_mat());
  p->set_uniform("view", view);
  p->set_uniform("projection", perspective);
  p->set_uniform("normalmat", get_normal_mat(), false);
  
  // Material properties
  p->set_uniform("material.ambient", material_.ambient);
  p->set_uniform("material.diffuse", material_.diffuse);
  p->set_uniform("material.specular", material_.specular);
  p->set_uniform("material.shininess", material_.shininess);

  p->activate();

  buf_.bind();
  normal_buf_.bind();

  glBindTexture(GL_TEXTURE_2D, 0);
  glDrawArrays(GL_TRIANGLES, 0, vertices_.rows());

  p->deactivate();

  normal_buf_.unbind();
  buf_.unbind();
}

void Plane::populate_bufs_() {
  vertices_ << -0.5f, -0.5f, 0.0f,  
                0.5f, -0.5f, 0.0f,  
                0.5f,  0.5f, 0.0f,  
                0.5f,  0.5f, 0.0f,  
               -0.5f,  0.5f, 0.0f,  
               -0.5f, -0.5f, 0.0f;

               // For two-sided plane
               //-0.5f, -0.5f, 0.0f, 
               // 0.5f, -0.5f, 0.0f, 
               // 0.5f,  0.5f, 0.0f, 
               // 0.5f,  0.5f, 0.0f, 
               //-0.5f,  0.5f, 0.0f, 
               //-0.5f, -0.5f, 0.0f;

    
  buf_.buffer(vertices_);
  
  normals_ << 0.0f,  0.0f, 1.0f,
              0.0f,  0.0f, 1.0f,
              0.0f,  0.0f, 1.0f,  
              0.0f,  0.0f, 1.0f,
              0.0f,  0.0f, 1.0f,
              0.0f,  0.0f, 1.0f;

              // For two-sided plane
              //0.0f,  0.0f, 1.0f,
              //0.0f,  0.0f, 1.0f,
              //0.0f,  0.0f, 1.0f,  
              //0.0f,  0.0f, 1.0f,
              //0.0f,  0.0f, 1.0f,
              //0.0f,  0.0f, 1.0f;

  normal_buf_.buffer(normals_);
}
