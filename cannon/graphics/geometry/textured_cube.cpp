#include <cannon/graphics/geometry/textured_cube.hpp>

#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/shader_program.hpp>

using namespace cannon::graphics::geometry;

TexturedCube::TexturedCube (ShaderProgramPtr p, const std::string& d_tex, const
    std::string& s_tex) : vao_(new VertexArrayObject), buf_(vao_),
  normal_buf_(vao_), texture_coord_buf_(vao_), diffuse_tex_(d_tex, false,
      GL_TEXTURE0), specular_tex_(s_tex, false, GL_TEXTURE1), vertices_(36, 3),
  normals_(36, 3), texture_coords_(36, 2) {

  program = p;

  populate_bufs_(); 

  name_ = std::string("Textured Cube");
}

TexturedCube::TexturedCube(TexturedCube& c) : vao_(new VertexArrayObject),
  buf_(vao_), normal_buf_(vao_), texture_coord_buf_(vao_),
  diffuse_tex_(c.diffuse_tex_), specular_tex_(c.specular_tex_),
  vertices_(c.vertices_), normals_(c.normals_),
  texture_coords_(c.texture_coords_) {

  program = c.program;

  buf_.buffer(vertices_);
  normal_buf_.buffer(normals_);
  texture_coord_buf_.buffer(texture_coords_);

  name_ = c.name_;

}

void TexturedCube::draw(const Matrix4f& view, const Matrix4f& perspective) const {
  program->set_uniform("model", get_model_mat());
  program->set_uniform("view", view);
  program->set_uniform("projection", perspective);
  program->set_uniform("normalmat", get_normal_mat(), false);
  
  // Material properties
  program->set_uniform("material.diffuse_tex[0]", 0);
  program->set_uniform("material.specular_tex[0]", 8);
  program->set_uniform("material.ambient", material_.ambient);
  program->set_uniform("material.diffuse", material_.diffuse);
  program->set_uniform("material.specular", material_.specular);
  program->set_uniform("material.shininess", material_.shininess);

  program->activate();

  diffuse_tex_.bind(GL_TEXTURE0);
  specular_tex_.bind(GL_TEXTURE8);

  buf_.bind();
  normal_buf_.bind();

  glDrawArrays(GL_TRIANGLES, 0, vertices_.rows());

  program->deactivate();

  specular_tex_.unbind(GL_TEXTURE8);
  diffuse_tex_.unbind();

  normal_buf_.unbind();
  buf_.unbind();
}

void TexturedCube::draw(std::shared_ptr<ShaderProgram> p, const Matrix4f& view, const
    Matrix4f& perspective) const {
  p->set_uniform("model", get_model_mat());
  p->set_uniform("view", view);
  p->set_uniform("projection", perspective);
  p->set_uniform("normalmat", get_normal_mat(), false);
  
  // Material properties
  p->set_uniform("material.ambient", material_.ambient);
  p->set_uniform("material.diffuse", material_.diffuse);
  p->set_uniform("material.specular", material_.specular);
  p->set_uniform("material.diffuse_tex[0]", 0);
  p->set_uniform("material.specular_tex[0]", 8);
  p->set_uniform("material.shininess", material_.shininess);

  p->activate();

  diffuse_tex_.bind(GL_TEXTURE0);
  specular_tex_.bind(GL_TEXTURE8);

  buf_.bind();
  normal_buf_.bind();

  glDrawArrays(GL_TRIANGLES, 0, vertices_.rows());

  p->deactivate();

  specular_tex_.unbind(GL_TEXTURE8);
  diffuse_tex_.unbind();

  normal_buf_.unbind();
  buf_.unbind();
}

void TexturedCube::populate_bufs_() {
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

  texture_coords_ << 0.0f, 0.0f,
                     1.0f, 1.0f,
                     1.0f, 0.0f,
                     1.0f, 1.0f,
                     0.0f, 0.0f,
                     0.0f, 1.0f,

                     0.0f, 0.0f,
                     1.0f, 0.0f,
                     1.0f, 1.0f,
                     1.0f, 1.0f,
                     0.0f, 1.0f,
                     0.0f, 0.0f,

                     1.0f, 0.0f,
                     1.0f, 1.0f,
                     0.0f, 1.0f,
                     0.0f, 1.0f,
                     0.0f, 0.0f,
                     1.0f, 0.0f,

                     1.0f, 0.0f,
                     0.0f, 1.0f,
                     1.0f, 1.0f,
                     0.0f, 1.0f,
                     1.0f, 0.0f,
                     0.0f, 0.0f,

                     0.0f, 1.0f,
                     1.0f, 1.0f,
                     1.0f, 0.0f,
                     1.0f, 0.0f,
                     0.0f, 0.0f,
                     0.0f, 1.0f,

                     0.0f, 1.0f,
                     1.0f, 0.0f,
                     1.0f, 1.0f,
                     1.0f, 0.0f,
                     0.0f, 1.0f,
                     0.0f, 0.0f;

  texture_coord_buf_.buffer(texture_coords_);
}
