#ifndef CANNON_PLOT_SCATTER_H
#define CANNON_PLOT_SCATTER_H 

#include <glad/glad.h>
#include <Eigen/Dense>

#include <cannon/graphics/window.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/vertex_shader.hpp>
#include <cannon/graphics/fragment_shader.hpp>
#include <cannon/log/registry.hpp>

using namespace Eigen;
using namespace cannon::graphics;
using namespace cannon::log;

namespace cannon {
  namespace plot {

    class Scatter {
      public:
        Scatter() = delete;

        Scatter(MatrixX2f points, Vector4f color, float point_size) : points_(points), color_(color), point_size_(point_size), buf_(vao_)  {
          glEnable(GL_PROGRAM_POINT_SIZE);
          buf_.buffer(points_);
          log_info(buf_);

          VertexShader v = load_vertex_shader("shaders/basic_2d_pass_pos.vert");
          FragmentShader f = load_fragment_shader("shaders/uniform_color_circle.frag");

          program_.attach_shader(v);
          program_.attach_shader(f);
          program_.link();
        }

        void add_points(MatrixX2f point);

        void draw();

        // TODO Delete
        void log();

      private:
        MatrixX2f points_;
        Vector4f color_;
        float point_size_;
        VertexArrayObject vao_;
        VertexBuffer buf_;
        ShaderProgram program_;
    };

  } // namespace plot
} // namespace cannon

#endif /* ifndef CANNON_PLOT_SCATTER_H */
