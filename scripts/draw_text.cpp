#ifdef CANNON_BUILD_GRAPHICS
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/text_renderer.hpp>
#include <cannon/graphics/font.hpp>

using namespace cannon::graphics;

int main() {
  Window w;
  w.set_clear_color({1.0, 1.0, 1.0, 1.0});

  auto font = std::make_shared<Font>();
  TextRenderer r(w.width, w.height, font);

  w.render_loop([&] {
      static int current_mode = 0;
      static float box_x = 25.0;
      static float box_y = 25.0;
      static float box_width = 200.0;
      static float box_height = 15.0;

      TextBoundingBox hello_box{box_x, box_y, box_width, box_height};

      switch (current_mode) {
        case 0:
          r.draw("Hello, world", hello_box, TextAlignment::Left);
          break;
        case 1:
          r.draw("Hello, world", hello_box, TextAlignment::Center);
          break;
        case 2:
          r.draw("Hello, world", hello_box, TextAlignment::Right);
          break;

      }

      if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Text")) {

          ImGui::SliderFloat("X", &box_x, 0.0, (float)w.width - box_width);
          ImGui::SliderFloat("Y", &box_y, 0.0, (float)w.height - box_height);
          ImGui::SliderFloat("Width", &box_width, 0.0, (float)w.width);
          ImGui::SliderFloat("Height", &box_height, 0.0, (float)w.height);

          static const char* modes[] = {"Left", "Center", "Right"};
          ImGui::Combo("Text Alignment", &current_mode, modes, IM_ARRAYSIZE(modes));


          ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
      }
    });
}
#else
int main() {}
#endif
