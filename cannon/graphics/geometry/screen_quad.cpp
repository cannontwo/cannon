#include <cannon/graphics/geometry/screen_quad.hpp>

#include <cannon/graphics/texture.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_buffer.hpp>

using namespace cannon::graphics::geometry;

ScreenQuad::ScreenQuad(std::shared_ptr<Texture> tex, int width, int height)
    : width(width), height(height), tex_(tex), vao_(new VertexArrayObject),
      buf_(new VertexBuffer(vao_)), texture_coord_buf_(new VertexBuffer(vao_)),
      vertices_(6, 2), texture_coords_(6, 2) {
  program = std::make_shared<ShaderProgram>("screen_quad_shader");
  program->attach_vertex_shader("shaders/pass_pos_tex.vert");
  program->attach_fragment_shader("shaders/tex_quad.frag");
  program->link();

  populate_quad_buf_();
}

void ScreenQuad::resize(int w, int h) {
  width = w;
  height = h;
}

void ScreenQuad::draw() {
    program->activate();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    tex_->bind();
    buf_->bind();
    texture_coord_buf_->bind();

    bool depth_enabled = glIsEnabled(GL_DEPTH_TEST);
    if (depth_enabled)
      glDisable(GL_DEPTH_TEST);
    
    glDrawArrays(GL_TRIANGLES, 0, 6);

    if (depth_enabled)
      glEnable(GL_DEPTH_TEST);

    tex_->unbind();
    texture_coord_buf_->unbind();
    buf_->unbind();
    program->deactivate();
}

void ScreenQuad::draw(std::shared_ptr<ShaderProgram> p) {
    p->activate();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    tex_->bind();
    buf_->bind();
    texture_coord_buf_->bind();

    bool depth_enabled = glIsEnabled(GL_DEPTH_TEST);
    if (depth_enabled)
      glDisable(GL_DEPTH_TEST);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    if (depth_enabled)
      glEnable(GL_DEPTH_TEST);

    tex_->unbind();
    texture_coord_buf_->unbind();
    buf_->unbind();
    p->deactivate();
}

void ScreenQuad::draw(std::shared_ptr<ShaderProgram> p,
    std::vector<std::shared_ptr<Texture>> textures) {
  p->activate();
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  for (unsigned int i = 0; i < textures.size(); i++) {
    textures[i]->bind(GL_TEXTURE0+i); 
  }

  buf_->bind();
  texture_coord_buf_->bind();

  bool depth_enabled = glIsEnabled(GL_DEPTH_TEST);
  if (depth_enabled)
    glDisable(GL_DEPTH_TEST);

  glDrawArrays(GL_TRIANGLES, 0, 6);

  if (depth_enabled)
    glEnable(GL_DEPTH_TEST);

  for (unsigned int i = 0; i < textures.size(); i++) {
    textures[i]->unbind(); 
  }

  texture_coord_buf_->unbind();
  buf_->unbind();
  p->deactivate();
}

void ScreenQuad::set_tex(std::shared_ptr<Texture> t) {
  tex_ = t;
}

void ScreenQuad::populate_quad_buf_() {
  vertices_ << -1.0f,  1.0f,
               -1.0f, -1.0f,
                1.0f, -1.0f,
                
               -1.0f,  1.0f,
                1.0f, -1.0f,
                1.0f,  1.0f;

  buf_->buffer(vertices_);

  texture_coords_ << 0.0f, 1.0f,
                     0.0f, 0.0f,
                     1.0f, 0.0f,
                     
                     0.0f, 1.0f,
                     1.0f, 0.0f,
                     1.0f, 1.0f;

  texture_coord_buf_->buffer(texture_coords_);
}
