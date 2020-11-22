#include <cannon/graphics/render_pass.hpp>

using namespace cannon::graphics;


void RenderPass::run() {
  glBeginQuery(GL_TIME_ELAPSED, gl_back_query_);

  framebuffer->bind();
  programs[0]->activate();
  draw_func();

  glEndQuery(GL_TIME_ELAPSED);

  unsigned int time;
  glGetQueryObjectuiv(gl_front_query_, GL_QUERY_RESULT, &time);
  set_time_taken((double)time / 1e9f);

  swap_queries_();

  programs[0]->deactivate();
  framebuffer->unbind();
};

void RenderPass::write_imgui() {
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("Render Passes")) {
      if (ImGui::TreeNode(name.c_str())) {
        ImGui::Text("Took %f seconds", time_taken_);
        ImGui::PlotLines("Time", time_cbuf_.data,
            IM_ARRAYSIZE(time_cbuf_.data), time_cbuf_.offset, NULL,
            0.0, 0.05);

        for (auto& p : programs)
          p->write_imgui(); 

        framebuffer->write_imgui();
        ImGui::TreePop();
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}

void RenderPass::set_time_taken(double t) {
  time_taken_ = t;
  time_cbuf_.add_point(t);
}

void RenderPass::swap_queries_() {
  unsigned int tmp = gl_front_query_;
  gl_front_query_ = gl_back_query_;
  gl_back_query_ = tmp;
}
