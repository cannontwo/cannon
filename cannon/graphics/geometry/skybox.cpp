#include <cannon/graphics/geometry/skybox.hpp>

using namespace cannon::graphics::geometry;

void Skybox::draw(const Matrix4f& view, const Matrix4f& perspective) {
  glDepthFunc(GL_LEQUAL);
  program->activate();
  
  Matrix4f rot_only = view;
  rot_only(0, 3) = 0.0;
  rot_only(1, 3) = 0.0;
  rot_only(2, 3) = 0.0;

  program->set_uniform("view", rot_only);
  program->set_uniform("projection", perspective);

  buf_.bind();
  cubemap_.bind(GL_TEXTURE0);

  glDrawArrays(GL_TRIANGLES, 0, vertices_.rows());
  glDepthFunc(GL_LESS);
}

void Skybox::populate_bufs_() {
  vertices_ << -1.0f,  1.0f, -1.0f,
               -1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
               -1.0f,  1.0f, -1.0f,

               -1.0f, -1.0f,  1.0f,
               -1.0f, -1.0f, -1.0f,
               -1.0f,  1.0f, -1.0f,
               -1.0f,  1.0f, -1.0f,
               -1.0f,  1.0f,  1.0f,
               -1.0f, -1.0f,  1.0f,

                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,

               -1.0f, -1.0f,  1.0f,
               -1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f, -1.0f,  1.0f,
               -1.0f, -1.0f,  1.0f,

               -1.0f,  1.0f, -1.0f,
                1.0f,  1.0f, -1.0f,
                1.0f,  1.0f,  1.0f,
                1.0f,  1.0f,  1.0f,
               -1.0f,  1.0f,  1.0f,
               -1.0f,  1.0f, -1.0f,

               -1.0f, -1.0f, -1.0f,
               -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
               -1.0f, -1.0f,  1.0f,
                1.0f, -1.0f,  1.0f;
    
  buf_.buffer(vertices_);
}

