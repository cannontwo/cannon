#ifndef CANNON_PLOT_AXES_H
#define CANNON_PLOT_AXES_H 

#include <string>
#include <Eigen/Dense>

#include <cannon/graphics/shader_program.hpp>
#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/font.hpp>
#include <cannon/utils/class_forward.hpp>

using namespace Eigen;

using namespace cannon::graphics;

namespace cannon {
  
  namespace graphics {
    CANNON_CLASS_FORWARD(VertexArrayObject);
  }

  namespace plot {

    class Axes {
      public:
        Axes(float text_scale=1.0, bool outside=false);

        void draw();
        void update_limits(float x_min, float x_max, float y_min, float y_max,
            int window_width, int window_height);
        void update_scale(int width, int height);
        void update_ticks();

        void set_xticks(std::vector<float> ticks);
        void set_yticks(std::vector<float> ticks);

        void write_imgui();

        Matrix4f make_scaling_matrix();
        // TODO Make separate scaling matrix for drawing actual axes, in order
        // to prevent overlapped plotting

        float x_min_;
        float x_max_;
        float y_min_;
        float y_max_;

        float text_offset_x_ = 0.0;
        float text_offset_y_ = 0.0;
        float axis_line_width_ = 1;

        bool outside_;
        float padding_;
        float text_scale_multiplier_;

      private:
        Vector2f get_scaled_padding();
        Matrix2f get_padded_extent();
        std::string get_tick_string(float tick);

        float text_scale_x_;
        float text_scale_y_;

        bool auto_ticks_ = true;
        std::vector<float> x_ticks_;
        std::vector<float> y_ticks_;

        ShaderProgram program_;
        VertexArrayObjectPtr vao_;
        VertexBuffer buf_;

        Font font_;
        ShaderProgram text_program_;
        VertexArrayObjectPtr text_vao_;
        VertexBuffer text_quad_buf_;
    };

  } // namespace plot
} // namespace cannon

#endif /* ifndef CANNON_PLOT_AXES_H */
