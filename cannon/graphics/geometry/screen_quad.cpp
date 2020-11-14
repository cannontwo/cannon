#include <cannon/graphics/geometry/screen_quad.hpp>

using namespace cannon::graphics::geometry;

void ScreenQuad::resize(int w, int h) {
  width = w;
  height = h;
}

void ScreenQuad::draw() {
    program->activate();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    tex_->bind();
    buf_.bind();
    texture_coord_buf_.bind();

    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
}

void ScreenQuad::draw(std::shared_ptr<ShaderProgram> p) {
    p->activate();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    tex_->bind();
    buf_.bind();
    texture_coord_buf_.bind();

    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
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

  buf_.buffer(vertices_);

  texture_coords_ << 0.0f, 1.0f,
                     0.0f, 0.0f,
                     1.0f, 0.0f,
                     
                     0.0f, 1.0f,
                     1.0f, 0.0f,
                     1.0f, 1.0f;

  texture_coord_buf_.buffer(texture_coords_);
}
