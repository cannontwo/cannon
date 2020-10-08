#ifndef CANNON_PLOT_AXES_H
#define CANNON_PLOT_AXES_H 

#include <cmath>
#include <Eigen/Dense>

#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/log/registry.hpp>

using namespace Eigen;

using namespace cannon::graphics;
using namespace cannon::log;

namespace cannon {
  namespace plot {

    class Axes {
      public:
        Axes() : x_min_(-1.0), x_max_(1.0), y_min_(-1.0), y_max_(1.0), buf_(vao_) {
          MatrixX2f lines(4, 2);
          lines << x_min_, 0.0,
                   x_max_, 0.0,
                   0.0, y_min_, 
                   0.0, y_max_; 
          buf_.buffer(lines);
          log_info(buf_);

          const char *v_src = BASIC_VERTEX_SHADER_2D.c_str();
          const char *f_src = BASIC_FRAGMENT_SHADER.c_str();
          VertexShader v(&v_src);
          FragmentShader f(&f_src);

          program_.attach_shader(v);
          program_.attach_shader(f);
          program_.link();
        }

        void draw();
        void update_limits(float x_min, float x_max, float y_min, float y_max);

      private:
        float x_min_;
        float x_max_;
        float y_min_;
        float y_max_;
        ShaderProgram program_;
        VertexArrayObject vao_;
        VertexBuffer buf_;

    };

  } // namespace plot
} // namespace cannon

#endif /* ifndef CANNON_PLOT_AXES_H */
