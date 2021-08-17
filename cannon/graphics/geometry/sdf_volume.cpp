#include <cannon/graphics/geometry/sdf_volume.hpp>

#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/vertex_buffer.hpp>

using namespace cannon::graphics::geometry;

SDFVolume::SDFVolume(std::shared_ptr<ShaderProgram> p)
    : vao_(new VertexArrayObject), buf_(new VertexBuffer(vao_)),
      vertices_(36, 3) {

  program = p;
  populate_bufs_();
  name_ = std::string("SDF Volume");

  material_.ambient = {1.0, 1.0, 1.0, 1.0};
  material_.diffuse = {1.0, 1.0, 1.0, 1.0};
  material_.specular = {0.0, 0.0, 0.0, 1.0};
}

SDFVolume::SDFVolume(SDFVolume &o)
    : vao_(new VertexArrayObject), buf_(new VertexBuffer(vao_)),
      vertices_(o.vertices_) {

  program = o.program;
  buf_->buffer(vertices_);

  name_ = o.name_;

  material_ = o.material_;
}

void SDFVolume::draw(const Matrix4f& view, const Matrix4f& perspective) const {
  program->set_uniform("model", get_model_mat());
  program->set_uniform("view", view);
  program->set_uniform("projection", perspective);
  program->set_uniform("normalmat", get_normal_mat(), false);

  program->set_uniform("scale", scale_);
  
  // Material properties
  program->set_uniform("material.ambient", material_.ambient);
  program->set_uniform("material.diffuse", material_.diffuse);
  program->set_uniform("material.specular", material_.specular);
  program->set_uniform("material.shininess", material_.shininess);

  program->activate();

  buf_->bind();

  glBindTexture(GL_TEXTURE_2D, 0);
  glDrawArrays(GL_TRIANGLES, 0, vertices_.rows());

  program->deactivate();

  buf_->unbind();
}

void SDFVolume::draw(std::shared_ptr<ShaderProgram> p, const Matrix4f& view, const
    Matrix4f& perspective) const {
  p->set_uniform("model", get_model_mat());
  p->set_uniform("view", view);
  p->set_uniform("projection", perspective);
  p->set_uniform("normalmat", get_normal_mat(), false);

  p->set_uniform("scale", scale_);
  
  // Material properties
  p->set_uniform("material.ambient", material_.ambient);
  p->set_uniform("material.diffuse", material_.diffuse);
  p->set_uniform("material.specular", material_.specular);
  p->set_uniform("material.shininess", material_.shininess);

  p->activate();

  buf_->bind();

  glBindTexture(GL_TEXTURE_2D, 0);
  glDrawArrays(GL_TRIANGLES, 0, vertices_.rows());

  p->deactivate();

  buf_->unbind();
}

void SDFVolume::populate_bufs_() {
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
    
  buf_->buffer(vertices_);
}
