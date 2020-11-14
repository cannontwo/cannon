#include <cannon/graphics/geometry/cube.hpp>

using namespace cannon::graphics::geometry;

void Cube::draw(const Matrix4f& view, const Matrix4f& perspective) const {
  program->activate();
  program->set_uniform("model", get_model_mat());
  program->set_uniform("view", view);
  program->set_uniform("projection", perspective);
  program->set_uniform("normalmat", get_normal_mat(), false);
  
  // Material properties
  program->set_uniform("material.ambient", material_.ambient);
  program->set_uniform("material.diffuse", material_.diffuse);
  program->set_uniform("material.specular", material_.specular);
  program->set_uniform("material.shininess", material_.shininess);

  buf_.bind();
  normal_buf_.bind();

  glDrawArrays(GL_TRIANGLES, 0, vertices_.rows());
}

void Cube::draw(std::shared_ptr<ShaderProgram> p, const Matrix4f& view, const
    Matrix4f& perspective) const {
  p->activate();
  p->set_uniform("model", get_model_mat());
  p->set_uniform("view", view);
  p->set_uniform("projection", perspective);
  p->set_uniform("normalmat", get_normal_mat(), false);
  
  // Material properties
  p->set_uniform("material.ambient", material_.ambient);
  p->set_uniform("material.diffuse", material_.diffuse);
  p->set_uniform("material.specular", material_.specular);
  p->set_uniform("material.shininess", material_.shininess);

  buf_.bind();
  normal_buf_.bind();

  glDrawArrays(GL_TRIANGLES, 0, vertices_.rows());

}

void Cube::populate_bufs_() {
  vertices_ << -0.5f, -0.5f, -0.5f,  
                0.5f,  0.5f, -0.5f,  
                0.5f, -0.5f, -0.5f,  
                0.5f,  0.5f, -0.5f,  
               -0.5f, -0.5f, -0.5f,  
               -0.5f,  0.5f, -0.5f,  

               -0.5f, -0.5f,  0.5f, 
                0.5f, -0.5f,  0.5f, 
                0.5f,  0.5f,  0.5f, 
                0.5f,  0.5f,  0.5f, 
               -0.5f,  0.5f,  0.5f, 
               -0.5f, -0.5f,  0.5f, 

               -0.5f,  0.5f,  0.5f, 
               -0.5f,  0.5f, -0.5f, 
               -0.5f, -0.5f, -0.5f, 
               -0.5f, -0.5f, -0.5f, 
               -0.5f, -0.5f,  0.5f, 
               -0.5f,  0.5f,  0.5f, 

                0.5f,  0.5f,  0.5f, 
                0.5f, -0.5f, -0.5f, 
                0.5f,  0.5f, -0.5f, 
                0.5f, -0.5f, -0.5f, 
                0.5f,  0.5f,  0.5f, 
                0.5f, -0.5f,  0.5f, 

               -0.5f, -0.5f, -0.5f, 
                0.5f, -0.5f, -0.5f, 
                0.5f, -0.5f,  0.5f, 
                0.5f, -0.5f,  0.5f, 
               -0.5f, -0.5f,  0.5f, 
               -0.5f, -0.5f, -0.5f, 

               -0.5f,  0.5f, -0.5f, 
                0.5f,  0.5f,  0.5f, 
                0.5f,  0.5f, -0.5f, 
                0.5f,  0.5f,  0.5f, 
               -0.5f,  0.5f, -0.5f,
               -0.5f,  0.5f,  0.5f; 
    
  buf_.buffer(vertices_);
  
  normals_ << 0.0f,  0.0f, -1.0f,
              0.0f,  0.0f, -1.0f,
              0.0f,  0.0f, -1.0f,  
              0.0f,  0.0f, -1.0f,
              0.0f,  0.0f, -1.0f,
              0.0f,  0.0f, -1.0f,
                                 
              0.0f,  0.0f, 1.0f,
              0.0f,  0.0f, 1.0f,
              0.0f,  0.0f, 1.0f,
              0.0f,  0.0f, 1.0f,
              0.0f,  0.0f, 1.0f,
              0.0f,  0.0f, 1.0f,
                                 
             -1.0f,  0.0f,  0.0f,
             -1.0f,  0.0f,  0.0f,
             -1.0f,  0.0f,  0.0f,
             -1.0f,  0.0f,  0.0f,
             -1.0f,  0.0f,  0.0f,
             -1.0f,  0.0f,  0.0f,
                                 
              1.0f,  0.0f,  0.0f,
              1.0f,  0.0f,  0.0f,
              1.0f,  0.0f,  0.0f,
              1.0f,  0.0f,  0.0f,
              1.0f,  0.0f,  0.0f,
              1.0f,  0.0f,  0.0f,
                                 
              0.0f, -1.0f,  0.0f,
              0.0f, -1.0f,  0.0f,
              0.0f, -1.0f,  0.0f,
              0.0f, -1.0f,  0.0f,
              0.0f, -1.0f,  0.0f,
              0.0f, -1.0f,  0.0f,
                                 
              0.0f,  1.0f,  0.0f,
              0.0f,  1.0f,  0.0f,
              0.0f,  1.0f,  0.0f,
              0.0f,  1.0f,  0.0f,
              0.0f,  1.0f,  0.0f,
              0.0f,  1.0f,  0.0f;

  normal_buf_.buffer(normals_);
}
