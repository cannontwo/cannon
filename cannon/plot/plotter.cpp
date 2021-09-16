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
using namespace cannon::graphics;

// Cribbed from Seaborn
std::vector<Vector4f> Plotter::LINE_COLORS = std::vector<Vector4f>(
    {{0.12156862745098039, 0.4666666666666667, 0.7058823529411765, 1.0},
     {1.0, 0.4980392156862745, 0.054901960784313725, 1.0},
     {0.17254901960784313, 0.6274509803921569, 0.17254901960784313, 1.0},
     {0.8392156862745098, 0.15294117647058825, 0.1568627450980392, 1.0},
     {0.5803921568627451, 0.403921568627451, 0.7411764705882353, 1.0},
     {0.5490196078431373, 0.33725490196078434, 0.29411764705882354, 1.0},
     {0.8901960784313725, 0.4666666666666667, 0.7607843137254902, 1.0},
     {0.4980392156862745, 0.4980392156862745, 0.4980392156862745, 1.0},
     {0.7372549019607844, 0.7411764705882353, 0.13333333333333333, 1.0},
     {0.09019607843137255, 0.7450980392156863, 0.8117647058823529, 1.0}});

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

std::shared_ptr<Polygon>
Plotter::plot_polygon(const std::vector<Vector2d> &poly,
                      const MatrixX4f &color) {
  auto p = std::make_shared<Polygon>(poly_program_, poly, color);
  polygon_plots_.push_back(p);

  RowVector2f maxes = p->points_.colwise().maxCoeff();
  RowVector2f mins = p->points_.colwise().minCoeff();
  axes_->update_limits(mins[0], maxes[0], mins[1], maxes[1], w_->width, w_->height);

  return p;
}

std::shared_ptr<Polygon>
Plotter::plot_polygon(const std::vector<Vector2d> &poly,
                      const Vector4f &color) {
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

  auto color = LINE_COLORS[line_plots_.size() % LINE_COLORS.size()];

  auto l = plot_line(points, color);
  return l;
}

LinePtr Plotter::plot(std::function<VectorXd(double)> f, unsigned int samples,
                      double low, double high) {
  VectorXd ts = VectorXd::LinSpaced(samples, low, high);
  MatrixX2f points(samples, 2);

  for (unsigned int i = 0; i < samples; ++i) {
    VectorXd pt = f(ts[i]);
    assert(pt.size() >= 2);

    points(i, 0) = pt[0];
    points(i, 1) = pt[1];
  }

  auto color = LINE_COLORS[line_plots_.size() % LINE_COLORS.size()];

  auto l = plot_line(points, color);
  return l;
}

LinePtr Plotter::plot(std::vector<double> points) {
  MatrixX2f plot_points(points.size(), 2);
  for (unsigned int i = 0; i < points.size(); ++i) {
    plot_points(i, 0) = i;
    plot_points(i, 1) = points[i];
  }

  auto color = LINE_COLORS[line_plots_.size() % LINE_COLORS.size()];

  auto l = plot_line(plot_points, color);
  return l;
}

LinePtr Plotter::plot(std::vector<Vector2d> points) {
  MatrixX2f plot_points(points.size(), 2);
  for (unsigned int i = 0; i < points.size(); ++i) {
    plot_points.row(i) = points[i].transpose().cast<float>();
  }

  auto color = LINE_COLORS[line_plots_.size() % LINE_COLORS.size()];

  auto l = plot_line(plot_points, color);
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

      std::vector<Vector2d> polygon;
      Vector2d cp1(x, y);
      polygon.push_back(cp1);
      Vector2d cp2(x + x_cell_side, y);
      polygon.push_back(cp2);
      Vector2d cp3(x + x_cell_side, y + y_cell_side);
      polygon.push_back(cp3);
      Vector2d cp4(x, y + y_cell_side);
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

  poly_program_->set_uniform("matrix", axes_->make_scaling_matrix());
  for (auto& poly : polygon_plots_) {
    poly->draw();
  }

  point_program_->set_uniform("matrix", axes_->make_scaling_matrix());
  for (auto& scatter : scatter_plots_) {
    scatter->draw();
  }

  line_program_->set_uniform("matrix", axes_->make_scaling_matrix());
  for (auto& line : line_plots_) {
    line->draw();
  }

  axes_->draw();
  axes_->write_imgui();
  write_imgui();
}

void Plotter::display_fps() {
  w_->display_fps(-25.0, -25.0, 1.0);
}

void Plotter::write_imgui() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("Plotting")) {
      if (ImGui::BeginMenu("Plots")) {
        bool point_changed = ImGui::SliderFloat("Scatter Plot Point Size",
            &scatter_point_size_, 1.0, 100.0);
        point_changed =
            point_changed ||
            ImGui::SliderFloat("Line Plot Width", &line_plot_size_, 1.0, 100.0);

        if (point_changed) {
          for (auto& s : scatter_plots_) {
            s->point_size_ = scatter_point_size_;
          }

          for (auto& l : line_plots_) {
            l->line_width_ = line_plot_size_;
          }

        }
        ImGui::EndMenu();
      }

      ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
  }
}
