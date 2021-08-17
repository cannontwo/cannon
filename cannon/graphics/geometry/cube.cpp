#include <cannon/graphics/geometry/cube.hpp>

#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/fragment_shader.hpp>
#include <cannon/graphics/vertex_shader.hpp>

using namespace cannon::graphics::geometry;

Cube::Cube(const std::string &v_src, const std::string &f_src)
    : vao_(new VertexArrayObject), buf_(new VertexBuffer(vao_)),
      normal_buf_(new VertexBuffer(vao_)), vertices_(36, 3), normals_(36, 3) {

  program = std::make_shared<ShaderProgram>();

  auto v = load_vertex_shader(v_src);
  auto f = load_fragment_shader(f_src);
  program->attach_shader(v);
  program->attach_shader(f);
  program->link();
  
  populate_bufs_();

  name_ = std::string("Cube");

  material_.ambient = {1.0, 1.0, 1.0, 1.0};
  material_.diffuse = {1.0, 1.0, 1.0, 1.0};
  material_.specular = {1.0, 1.0, 1.0, 1.0};
}

// Does not affect OpenGL state
Cube::Cube(std::shared_ptr<ShaderProgram> p)
    : vao_(new VertexArrayObject), buf_(new VertexBuffer(vao_)),
      normal_buf_(new VertexBuffer(vao_)), vertices_(36, 3), normals_(36, 3) {

  program = p;

  populate_bufs_(); 

  name_ = std::string("Cube");
  
  material_.ambient = {1.0, 1.0, 1.0, 1.0};
  material_.diffuse = {1.0, 1.0, 1.0, 1.0};
  material_.specular = {0.0, 0.0, 0.0, 1.0};

}

Cube::Cube(Cube &c)
    : vao_(new VertexArrayObject), buf_(new VertexBuffer(vao_)),
      normal_buf_(new VertexBuffer(vao_)), vertices_(c.vertices_),
      normals_(c.normals_) {
  program = c.program;
  buf_->buffer(vertices_);
  normal_buf_->buffer(normals_);

  name_ = c.name_;

  material_ = c.material_;
}

void Cube::draw(const Matrix4f& view, const Matrix4f& perspective) const {
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

  buf_->bind();
  normal_buf_->bind();

  glBindTexture(GL_TEXTURE_2D, 0);
  glDrawArrays(GL_TRIANGLES, 0, vertices_.rows());

  program->deactivate();
  buf_->unbind();
  normal_buf_->unbind();
}

void Cube::draw(std::shared_ptr<ShaderProgram> p, const Matrix4f& view, const
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

  buf_->bind();
  normal_buf_->bind();

  glBindTexture(GL_TEXTURE_2D, 0);
  glDrawArrays(GL_TRIANGLES, 0, vertices_.rows());

  p->deactivate();
  buf_->unbind();
  normal_buf_->unbind();
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
    
  buf_->buffer(vertices_);
  
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

  normal_buf_->buffer(normals_);
}
