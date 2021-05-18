#include <cannon/plot/plotter.hpp>

#include <stdexcept>

#include <stb_image/stb_image_write.h>

#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>
#include <cannon/plot/scatter.hpp>
#include <cannon/plot/polygon.hpp>
#include <cannon/plot/line.hpp>

using namespace cannon::plot;

Plotter::Plotter(bool axes_outside) : w_(), axes_(1.0, axes_outside),
  axes_outside_(axes_outside), point_program_(new ShaderProgram),
  line_program_(new ShaderProgram), poly_program_(new ShaderProgram)  {

  //glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //glDisable(GL_DEPTH_TEST);

  w_.set_clear_color(Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
  w_.disable_depth_test();
  w_.disable_face_culling();
  w_.disable_stencil_test();

  // Set up point shader
  VertexShader v = load_vertex_shader("shaders/basic_2d_pass_pos.vert");
  FragmentShader f = load_fragment_shader("shaders/uniform_color_circle.frag");

  point_program_->attach_shader(v);
  point_program_->attach_shader(f);
  point_program_->link();

  // Set up line shader
  const char *fl_src = BASIC_FRAGMENT_SHADER.c_str();
  VertexShader vl = load_vertex_shader("shaders/basic_2d_pass_pos.vert");
  FragmentShader fl = FragmentShader(&fl_src);

  line_program_->attach_shader(vl);
  line_program_->attach_shader(fl);
  line_program_->link();


  // TODO Make Polygon vertex shader with per-vertex colors
  VertexShader poly_vl = load_vertex_shader("shaders/2d_pass_pos_color.vert");
  FragmentShader poly_fl = load_fragment_shader("shaders/pass_color_viridis.frag");
  poly_program_->attach_shader(poly_vl);
  poly_program_->attach_shader(poly_fl);
  poly_program_->link();
}

void Plotter::close() const {
  w_.close();
}

ImageData Plotter::get_image() const {
  return w_.get_image();
}

void Plotter::save_image(const std::string& path) {

  w_.render_loop([this, path] {
      draw_pass();
      static int i = 0;
      i++;

      if (i == 5) {
        auto data = get_image();
        stbi_flip_vertically_on_write(true);
        if (stbi_write_png(path.c_str(), data.width, data.height,
              data.num_channels, data.buffer->data(), data.stride) == 0) {
          throw std::runtime_error("Could not write Plotter render to file"); 
        }

        w_.close();
      }
    });
}

void Plotter::render() {
  w_.render_loop([this] {
        draw_pass();
      });
}

std::shared_ptr<Scatter> Plotter::plot_points(MatrixX2f points, Vector4f color, float size) {
  auto s = std::make_shared<Scatter>(*this, point_program_, points, color, size);
  scatter_plots_.push_back(s);

  // Update plot extent
  RowVector2f maxes = points.colwise().maxCoeff();
  RowVector2f mins = points.colwise().minCoeff();
  axes_.update_limits(mins[0], maxes[0], mins[1], maxes[1], w_.width, w_.height);

  return s;
}

std::shared_ptr<Line> Plotter::plot_line(MatrixX2f points, Vector4f color) {
  auto l = std::make_shared<Line>(*this, line_program_, points, color);
  line_plots_.push_back(l);

  // Update plot extent
  RowVector2f maxes = points.colwise().maxCoeff();
  RowVector2f mins = points.colwise().minCoeff();
  axes_.update_limits(mins[0], maxes[0], mins[1], maxes[1], w_.width, w_.height);

  return l;
}

std::shared_ptr<Polygon> Plotter::plot_polygon(const Polygon_2& poly, const MatrixX4f& color) {
  auto p = std::make_shared<Polygon>(poly_program_, poly, color);
  polygon_plots_.push_back(p);

  RowVector2f maxes = p->points_.colwise().maxCoeff();
  RowVector2f mins = p->points_.colwise().minCoeff();
  axes_.update_limits(mins[0], maxes[0], mins[1], maxes[1], w_.width, w_.height);

  return p;
}

std::shared_ptr<Polygon> Plotter::plot_polygon(const Polygon_2& poly, const Vector4f& color) {
  auto p = std::make_shared<Polygon>(poly_program_, poly, color);
  polygon_plots_.push_back(p);

  RowVector2f maxes = p->points_.colwise().maxCoeff();
  RowVector2f mins = p->points_.colwise().minCoeff();
  axes_.update_limits(mins[0], maxes[0], mins[1], maxes[1], w_.width, w_.height);

  return p;
}

void Plotter::set_xlim(float low, float high) {
  axes_.update_limits(low, high, 0.0, 0.0, w_.width, w_.height);
}

void Plotter::set_ylim(float low, float high) {
  axes_.update_limits(0.0, 0.0, low, high, w_.width, w_.height);
}

void Plotter::draw_pass() {
  axes_.update_scale(w_.width, w_.height);
  axes_.draw();

  point_program_->set_uniform("matrix", axes_.make_scaling_matrix());
  for (auto& scatter : scatter_plots_) {
    scatter->draw();
  }

  line_program_->set_uniform("matrix", axes_.make_scaling_matrix());
  for (auto& line : line_plots_) {
    line->draw();
  }

  poly_program_->set_uniform("matrix", axes_.make_scaling_matrix());
  for (auto& poly : polygon_plots_) {
    poly->draw();
  }

  write_imgui();
}

void Plotter::display_fps() {
  w_.display_fps(-25.0, -25.0, 1.0);
}

void Plotter::write_imgui() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("Plotting")) {
      ImGui::Checkbox("Axes Outside", &axes_.outside_);
      ImGui::SliderFloat("Text Scale", &axes_.text_scale_multiplier_, 0.1, 5.0);
      ImGui::SliderFloat("Padding", &axes_.padding_, 0.01, 1.0);

      ImGui::SliderFloat("X Ticks Offset", &axes_.text_offset_y_, -1.0, 1.0);
      ImGui::SliderFloat("Y Ticks Offset", &axes_.text_offset_x_, -1.0, 1.0);
      ImGui::SliderFloat("Axis Width", &axes_.axis_line_width_, 0.1, 10.0);

      bool changed = false;

      changed = changed || ImGui::InputFloat("X Min", &axes_.x_min_);
      changed = changed || ImGui::InputFloat("X Max", &axes_.x_max_);
      changed = changed || ImGui::InputFloat("Y Min", &axes_.y_min_);
      changed = changed || ImGui::InputFloat("Y Max", &axes_.y_max_);

      if (changed) {
        axes_.update_limits(axes_.x_min_, axes_.x_max_, axes_.y_min_,
            axes_.y_max_, w_.width, w_.height);
      }

      if (ImGui::BeginMenu("Plots")) {
        bool point_changed = ImGui::SliderFloat("Scatter Plot Point Size",
            &scatter_point_size_, 1.0, 100.0);

        if (point_changed) {
          for (auto& s : scatter_plots_) {
            s->point_size_ = scatter_point_size_;
          }

        }
        ImGui::EndMenu();
      }

      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }
}
