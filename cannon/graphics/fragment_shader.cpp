#include <cannon/graphics/fragment_shader.hpp>

using namespace cannon::graphics;

FragmentShader cannon::graphics::load_fragment_shader(const std::string& path) {
  try {
    std::ifstream fs(path);

    // Read everything into str
    std::string str((std::istreambuf_iterator<char>(fs)),
        std::istreambuf_iterator<char>());

    log_info(str);

    const char *c_str = str.c_str();
    return FragmentShader(&c_str);
  } catch (std::ifstream::failure e) {
    throw std::runtime_error("Couldn't open file for fragment shader source");
  }
}
