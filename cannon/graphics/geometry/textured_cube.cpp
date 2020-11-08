#include <cannon/graphics/geometry/textured_cube.hpp>

using namespace cannon::graphics::geometry;

void TexturedCube::draw(const Matrix4f& view, const Matrix4f& perspective) const {
  program->activate();
  program->set_uniform("model", get_model_mat());
  program->set_uniform("view", view);
  program->set_uniform("projection", perspective);
  program->set_uniform("normalmat", get_normal_mat(), false);
  
  // Material properties
  program->set_uniform("material.diffuse[0]", 0);
  program->set_uniform("material.specular[0]", 8);
  program->set_uniform("material.shininess", material_.shininess);

  diffuse_tex_.bind(GL_TEXTURE0);
  specular_tex_.bind(GL_TEXTURE8);

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
