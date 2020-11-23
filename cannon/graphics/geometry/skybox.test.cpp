#include <cannon/graphics/geometry/skybox.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

int main() {
  Window w;
  OpenGLState s;

  std::vector<std::string> faces;
  faces.push_back("assets/skybox/right.jpg");
  faces.push_back("assets/skybox/left.jpg");
  faces.push_back("assets/skybox/top.jpg");
  faces.push_back("assets/skybox/bottom.jpg");
  faces.push_back("assets/skybox/front.jpg");
  faces.push_back("assets/skybox/back.jpg");

  geometry::Skybox v(faces);
  OpenGLState s1;
  assert(s == s1);

  Matrix4f tmp = Matrix4f::Zero();
  v.draw(tmp, tmp);
  OpenGLState s2;
  assert(s == s2);
}
