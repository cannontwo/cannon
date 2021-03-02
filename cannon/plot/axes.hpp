#ifndef CANNON_PLOT_AXES_H
#define CANNON_PLOT_AXES_H 

#include <cmath>
#include <string>
#include <sstream>
#include <Eigen/Dense>

#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/font.hpp>
#include <cannon/log/registry.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace Eigen;

using namespace cannon::graphics;
using namespace cannon::log;

namespace cannon {
  namespace plot {

    class Axes {
      public:
        Axes(float text_scale=0.1) : x_min_(-0.01), x_max_(0.01), y_min_(-0.01), y_max_(0.01),
            padding_(0.1), text_scale_x_(text_scale), text_scale_y_(text_scale), vao_(new VertexArrayObject), buf_(vao_),
            font_(true, 12), text_vao_(new VertexArrayObject), text_quad_buf_(text_vao_)  {


          MatrixX2f lines(4, 2);
          lines << x_min_, 0.0,
                   x_max_, 0.0,
                   0.0, y_min_, 
                   0.0, y_max_; 
          buf_.buffer(lines);
          log_info(buf_);

          x_ticks_.push_back(-1.0);
          x_ticks_.push_back(1.0);
          y_ticks_.push_back(-1.0);
          y_ticks_.push_back(1.0);

          const char *v_src = BASIC_VERTEX_SHADER_2D.c_str();
          const char *f_src = BASIC_FRAGMENT_SHADER.c_str();
          VertexShader v(&v_src);
          FragmentShader f(&f_src);

          program_.attach_shader(v);
          program_.attach_shader(f);
          program_.link();

          VertexShader tv = load_vertex_shader("shaders/char_shader.vert");
          FragmentShader tf = load_fragment_shader("shaders/char_shader.frag");

          text_program_.attach_shader(tv);
          text_program_.attach_shader(tf);
          text_program_.link();
          
          MatrixX4f vertices(MatrixX4f::Zero(6, 4));
          text_quad_buf_.buffer(vertices);
        }

        void draw();
        void update_limits(float x_min, float x_max, float y_min, float y_max,
            int window_width, int window_height);
        void update_scale(int width, int height);
        void update_ticks();

        void set_xticks(std::vector<float> ticks);
        void set_yticks(std::vector<float> ticks);

        Matrix4f make_scaling_matrix();

        float x_min_;
        float x_max_;
        float y_min_;
        float y_max_;

      private:
        Vector2f get_scaled_padding();
        Matrix2f get_padded_extent();
        std::string get_tick_string(float tick);

        float padding_;
        float text_scale_x_;
        float text_scale_y_;

        bool auto_ticks_ = true;
        std::vector<float> x_ticks_;
        std::vector<float> y_ticks_;

        ShaderProgram program_;
        std::shared_ptr<VertexArrayObject> vao_;
        VertexBuffer buf_;

        Font font_;
        ShaderProgram text_program_;
        std::shared_ptr<VertexArrayObject> text_vao_;
        VertexBuffer text_quad_buf_;
    };

  } // namespace plot
} // namespace cannon

#endif /* ifndef CANNON_PLOT_AXES_H */
