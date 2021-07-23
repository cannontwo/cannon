#include <cannon/plot/plotter.hpp>

#include <stdexcept>

#include <stb_image/stb_image_write.h>

#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/plot/axes.hpp>
#include <cannon/plot/scatter.hpp>
#include <cannon/plot/polygon.hpp>
#include <cannon/plot/line.hpp>

using namespace cannon::plot;

Plotter::Plotter(bool axes_outside) : w_(new Window()), axes_(new Axes(1.0, axes_outside)),
  axes_outside_(axes_outside), point_program_(new ShaderProgram),
  line_program_(new ShaderProgram), poly_program_(new ShaderProgram)  {

  //glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  //glDisable(GL_DEPTH_TEST);

  w_->set_clear_color(Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
  w_->disable_depth_test();
  w_->disable_face_culling();
  w_->disable_stencil_test();

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
  w_->close();
}

ImageData Plotter::get_image() const {
  return w_->get_image();
}

void Plotter::save_image(const std::string& path) {

  w_->render_loop([this, path] {
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

        w_->close();
      }
    });
}

void Plotter::render() {
  w_->render_loop([this] {
        draw_pass();
      });
}

void Plotter::render(std::function<void()> f) {
  w_->render_loop([this, f] {
    draw_pass();
    f();
  });
}

void Plotter::clear() {
  line_plots_.clear();
  scatter_plots_.clear();
  polygon_plots_.clear();
}

std::shared_ptr<Scatter> Plotter::plot_points(MatrixX2f points, Vector4f color, float size) {
  auto s = std::make_shared<Scatter>(*this, point_program_, points, color, size);
  scatter_plots_.push_back(s);

  // Update plot extent
  RowVector2f maxes = points.colwise().maxCoeff();
  RowVector2f mins = points.colwise().minCoeff();
  axes_->update_limits(mins[0], maxes[0], mins[1], maxes[1], w_->width, w_->height);

  return s;
}

std::shared_ptr<Line> Plotter::plot_line(MatrixX2f points, Vector4f color) {
  auto l = std::make_shared<Line>(*this, line_program_, points, color);
  line_plots_.push_back(l);

  // Update plot extent
  RowVector2f maxes = points.colwise().maxCoeff();
  RowVector2f mins = points.colwise().minCoeff();
  axes_->update_limits(mins[0], maxes[0], mins[1], maxes[1], w_->width, w_->height);

  return l;
}

std::shared_ptr<Polygon> Plotter::plot_polygon(const Polygon_2& poly, const MatrixX4f& color) {
  auto p = std::make_shared<Polygon>(poly_program_, poly, color);
  polygon_plots_.push_back(p);

  RowVector2f maxes = p->points_.colwise().maxCoeff();
  RowVector2f mins = p->points_.colwise().minCoeff();
  axes_->update_limits(mins[0], maxes[0], mins[1], maxes[1], w_->width, w_->height);

  return p;
}

std::shared_ptr<Polygon> Plotter::plot_polygon(const Polygon_2& poly, const Vector4f& color) {
  auto p = std::make_shared<Polygon>(poly_program_, poly, color);
  polygon_plots_.push_back(p);

  RowVector2f maxes = p->points_.colwise().maxCoeff();
  RowVector2f mins = p->points_.colwise().minCoeff();
  axes_->update_limits(mins[0], maxes[0], mins[1], maxes[1], w_->width, w_->height);

  return p;
}

LinePtr Plotter::plot(std::function<double(double)> f, unsigned int samples,
                      double low, double high) {
  VectorXd xs = VectorXd::LinSpaced(samples, low, high);
  MatrixX2f points(samples, 2);
  points.col(0) = xs.cast<float>();

  for (unsigned int i = 0; i < samples; ++i) {
    points(i, 1) = f(xs[i]);
  }

  auto l = plot_line(points);
  return l;
}

void Plotter::plot(std::function<double(const Vector2d &)> f,
                   unsigned int lattice_dim, double x_low, double x_high,
                   double y_low, double y_high) {
  assert(x_high > x_low);
  assert(y_high > y_low);

  double x_extent = x_high - x_low;
  double x_cell_side = x_extent / lattice_dim;

  double y_extent = y_high - y_low;
  double y_cell_side = y_extent / lattice_dim;

  MatrixXd vals = MatrixXd::Zero(lattice_dim+1, lattice_dim+1);

  double max_val = -std::numeric_limits<double>::infinity();
  double min_val = std::numeric_limits<double>::infinity();
  for (unsigned int i = 0; i < lattice_dim + 1; ++i) {
    for (unsigned int j = 0; j < lattice_dim + 1; ++j) {
      double x = x_low + (x_extent / lattice_dim) * i;
      double y = y_low + (y_extent / lattice_dim) * j;

      Vector2d vec;
      vec << x, y;

      vals(i, j) = f(vec);

      if (vals(i, j) > max_val)
        max_val = vals(i, j);
      if (vals(i, j) < min_val)
        min_val = vals(i, j);
    }
  }

  MatrixX4f colors = MatrixX4f::Zero(4, 4);

  for (unsigned int i = 0; i < lattice_dim; ++i) {
    for (unsigned int j = 0; j < lattice_dim; ++j) {
      Vector4f point_value_color = Vector4f::Ones();
      point_value_color[0] = (vals(i, j) - min_val) / (max_val - min_val);
      colors.row(0) = point_value_color.transpose();

      point_value_color[0] = (vals(i + 1, j) - min_val) / (max_val - min_val);
      colors.row(1) = point_value_color.transpose();

      point_value_color[0] = (vals(i + 1, j + 1) - min_val) / (max_val - min_val);
      colors.row(2) = point_value_color.transpose();

      point_value_color[0] = (vals(i, j + 1) - min_val) / (max_val - min_val);
      colors.row(3) = point_value_color.transpose();

      double x = x_low + (x_extent / lattice_dim) * i;
      double y = y_low + (y_extent / lattice_dim) * j;

      Polygon_2 polygon;
      K::Point_2 cp1(x, y);
      polygon.push_back(cp1);
      K::Point_2 cp2(x + x_cell_side, y);
      polygon.push_back(cp2);
      K::Point_2 cp3(x + x_cell_side, y + y_cell_side);
      polygon.push_back(cp3);
      K::Point_2 cp4(x, y + y_cell_side);
      polygon.push_back(cp4);

      plot_polygon(polygon, colors);
    }
  }
}

void Plotter::set_xlim(float low, float high) {
  axes_->update_limits(low, high, 0.0, 0.0, w_->width, w_->height);
}

void Plotter::set_ylim(float low, float high) {
  axes_->update_limits(0.0, 0.0, low, high, w_->width, w_->height);
}

void Plotter::draw_pass() {
  axes_->update_scale(w_->width, w_->height);

  point_program_->set_uniform("matrix", axes_->make_scaling_matrix());
  for (auto& scatter : scatter_plots_) {
    scatter->draw();
  }

  line_program_->set_uniform("matrix", axes_->make_scaling_matrix());
  for (auto& line : line_plots_) {
    line->draw();
  }

  poly_program_->set_uniform("matrix", axes_->make_scaling_matrix());
  for (auto& poly : polygon_plots_) {
    poly->draw();
  }

  axes_->draw();

  write_imgui();
}

void Plotter::display_fps() {
  w_->display_fps(-25.0, -25.0, 1.0);
}

void Plotter::write_imgui() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("Plotting")) {
      ImGui::SliderFloat("Text Scale", &axes_->text_scale_multiplier_, 0.1, 5.0);
      ImGui::SliderFloat("Padding", &axes_->padding_, 0.01, 1.0);

      ImGui::SliderFloat("X Ticks Offset", &axes_->text_offset_y_, -1.0, 1.0);
      ImGui::SliderFloat("Y Ticks Offset", &axes_->text_offset_x_, -1.0, 1.0);
      ImGui::SliderFloat("Axis Width", &axes_->axis_line_width_, 0.1, 10.0);

      bool changed = false;

      changed = changed || ImGui::InputFloat("X Min", &axes_->x_min_);
      changed = changed || ImGui::InputFloat("X Max", &axes_->x_max_);
      changed = changed || ImGui::InputFloat("Y Min", &axes_->y_min_);
      changed = changed || ImGui::InputFloat("Y Max", &axes_->y_max_);
      changed = changed || ImGui::Checkbox("Axes Outside", &axes_->outside_);

      if (changed) {
        axes_->update_limits(axes_->x_min_, axes_->x_max_, axes_->y_min_,
            axes_->y_max_, w_->width, w_->height);
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
