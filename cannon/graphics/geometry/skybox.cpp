#include <cannon/graphics/geometry/skybox.hpp>

#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/cubemap.hpp>

using namespace cannon::graphics::geometry;

Skybox::Skybox(std::vector<std::string> face_paths, const std::string& v_src,
    const std::string& f_src) : cubemap_(new Cubemap(face_paths)), vao_(new
      VertexArrayObject), buf_(new VertexBuffer(vao_)), vertices_(36, 3) {

  program = std::make_shared<ShaderProgram>("skybox_shader");
  program->attach_vertex_shader(v_src);
  program->attach_fragment_shader(f_src);
  program->link();

  populate_bufs_();

}

void Skybox::draw(const Matrix4f& view, const Matrix4f& perspective) {
  glDepthFunc(GL_LEQUAL);
  
  Matrix4f rot_only = view;
  rot_only(0, 3) = 0.0;
  rot_only(1, 3) = 0.0;
  rot_only(2, 3) = 0.0;

  program->set_uniform("view", rot_only);
  program->set_uniform("projection", perspective);

  program->activate();

  buf_->bind();
  cubemap_->bind(GL_TEXTURE0);

  glDrawArrays(GL_TRIANGLES, 0, vertices_.rows());
  glDepthFunc(GL_LESS);

  program->deactivate();

  cubemap_->unbind();
  buf_->unbind();
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
    
  buf_->buffer(vertices_);
}

