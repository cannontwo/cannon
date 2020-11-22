#include <cannon/graphics/geometry/axes_hint.hpp>

using namespace cannon::graphics::geometry;

void AxesHint::draw(const Matrix4f& view, const Matrix4f& perspective) const {
  program->set_uniform("model", get_model_mat());
  program->set_uniform("view", view);
  program->set_uniform("projection", perspective);


  // Basically just draw three lines with different materials (red, green, blue) for (x, y, z)
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  buf_x_.bind();
  program->set_uniform("uColor", Vector4f{1.0, 0.0, 0.0, 0.5});
  program->activate();
  glDrawArrays(GL_LINES, 0, vertices_x_.rows());

  buf_y_.bind();
  program->set_uniform("uColor", Vector4f{0.0, 1.0, 0.0, 0.5});
  program->activate();
  glDrawArrays(GL_LINES, 0, vertices_y_.rows());

  buf_z_.bind();
  program->set_uniform("uColor", Vector4f{0.0, 0.0, 1.0, 0.5});
  program->activate();
  glDrawArrays(GL_LINES, 0, vertices_z_.rows());
  glDisable(GL_BLEND);

  buf_z_.unbind();
}

void AxesHint::draw(std::shared_ptr<ShaderProgram> p, const Matrix4f& view, const
    Matrix4f& perspective) const {
  p->set_uniform("model", get_model_mat());
  p->set_uniform("view", view);
  p->set_uniform("projection", perspective);

  // Basically just draw three lines with different materials (red, green, blue) for (x, y, z)
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  buf_x_.bind();
  p->set_uniform("uColor", Vector4f{1.0, 0.0, 0.0, 0.5});
  p->activate();
  glDrawArrays(GL_LINES, 0, vertices_x_.rows());

  buf_y_.bind();
  p->set_uniform("uColor", Vector4f{0.0, 1.0, 0.0, 0.5});
  p->activate();
  glDrawArrays(GL_LINES, 0, vertices_y_.rows());

  buf_z_.bind();
  p->set_uniform("uColor", Vector4f{0.0, 0.0, 1.0, 0.5});
  p->activate();
  glDrawArrays(GL_LINES, 0, vertices_z_.rows());
  glDisable(GL_BLEND);

  buf_z_.unbind();
}

void AxesHint::populate_bufs_x_() {
  vertices_x_ << 0.0f, 0.0f, 0.0f,
                1.0f, 0.0f, 0.0f;

  buf_x_.buffer(vertices_x_);
}

void AxesHint::populate_bufs_y_() {
  vertices_y_ <<  0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f;

  buf_y_.buffer(vertices_y_);
}

void AxesHint::populate_bufs_z_() {
  vertices_z_ <<  0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f;

  buf_z_.buffer(vertices_z_);
}
