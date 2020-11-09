#include <cannon/graphics/vertex_shader.hpp>

using namespace cannon::graphics;

VertexShader cannon::graphics::load_vertex_shader(const std::string& path) {
  try {
    std::ifstream fs(path);

    // Read everything into str
    std::string str((std::istreambuf_iterator<char>(fs)),
        std::istreambuf_iterator<char>());

    const char *c_str = str.c_str();
    return VertexShader(&c_str);
  } catch (std::ifstream::failure e) {
    throw std::runtime_error("Couldn't open file for vertex shader source");
  }
}
