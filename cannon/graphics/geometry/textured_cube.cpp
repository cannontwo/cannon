#include <cannon/graphics/geometry/textured_cube.hpp>

using namespace cannon::graphics::geometry;

void TexturedCube::draw(const Matrix4f& view, const Matrix4f& perspective) const {
  program_->activate();
  program_->set_uniform("model", get_model_mat());
  program_->set_uniform("view", view);
  program_->set_uniform("projection", perspective);
  program_->set_uniform("normalmat", get_normal_mat(), false);
  
  // Material properties
  program_->set_uniform("material.diffuse", 0);
  program_->set_uniform("material.specular", 1);
  program_->set_uniform("material.shininess", material_.shininess);

  diffuse_tex_.bind(GL_TEXTURE0);
  specular_tex_.bind(GL_TEXTURE1);

  buf_.bind();
  normal_buf_.bind();

  glDrawArrays(GL_TRIANGLES, 0, vertices_.rows());
}

void TexturedCube::populate_bufs_() {
  vertices_ << -0.5f, -0.5f, -0.5f,  
                0.5f, -0.5f, -0.5f,  
                0.5f,  0.5f, -0.5f,  
                0.5f,  0.5f, -0.5f,  
               -0.5f,  0.5f, -0.5f,  
               -0.5f, -0.5f, -0.5f,  

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
                0.5f,  0.5f, -0.5f, 
                0.5f, -0.5f, -0.5f, 
                0.5f, -0.5f, -0.5f, 
                0.5f, -0.5f,  0.5f, 
                0.5f,  0.5f,  0.5f, 

               -0.5f, -0.5f, -0.5f, 
                0.5f, -0.5f, -0.5f, 
                0.5f, -0.5f,  0.5f, 
                0.5f, -0.5f,  0.5f, 
               -0.5f, -0.5f,  0.5f, 
               -0.5f, -0.5f, -0.5f, 

               -0.5f,  0.5f, -0.5f, 
                0.5f,  0.5f, -0.5f, 
                0.5f,  0.5f,  0.5f, 
                0.5f,  0.5f,  0.5f, 
               -0.5f,  0.5f,  0.5f, 
               -0.5f,  0.5f, -0.5f;
    
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
                     1.0f, 0.0f,
                     1.0f, 1.0f,
                     1.0f, 1.0f,
                     0.0f, 1.0f,
                     0.0f, 0.0f,

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
                     1.0f, 1.0f,
                     0.0f, 1.0f,
                     0.0f, 1.0f,
                     0.0f, 0.0f,
                     1.0f, 0.0f,

                     0.0f, 1.0f,
                     1.0f, 1.0f,
                     1.0f, 0.0f,
                     1.0f, 0.0f,
                     0.0f, 0.0f,
                     0.0f, 1.0f,

                     0.0f, 1.0f,
                     1.0f, 1.0f,
                     1.0f, 0.0f,
                     1.0f, 0.0f,
                     0.0f, 0.0f,
                     0.0f, 1.0f;
  texture_coord_buf_.buffer(texture_coords_);
}
