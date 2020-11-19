#include <cannon/graphics/fragment_shader.hpp>

using namespace cannon::graphics;

FragmentShader cannon::graphics::load_fragment_shader(const std::string& path) {
  try {
    std::ifstream fs(path);

    // Read everything into str
    std::string str((std::istreambuf_iterator<char>(fs)),
        std::istreambuf_iterator<char>());

    const char *c_str = str.c_str();
    return FragmentShader(&c_str);
  } catch (std::ifstream::failure e) {
    throw std::runtime_error("Couldn't open file for fragment shader source");
  }
}

FragmentShader cannon::graphics::load_fragment_shader(const std::string& main_path,
    const std::vector<std::string>& lib_paths) {
  std::string program_src;

  try {
    std::ifstream fs(main_path);

    // Read everything into str
    program_src = std::string((std::istreambuf_iterator<char>(fs)),
        std::istreambuf_iterator<char>());

  } catch (std::ifstream::failure e) {
    throw std::runtime_error("Couldn't open file for fragment shader source");
  }

  // We insert after the first line so that the GLSL version statement from the
  // main program is still the first line
  std::size_t second_line_loc = program_src.find('\n');
  if (second_line_loc == std::string::npos)
    throw std::runtime_error("Main shader program consists of only a single line.");

  log_info("lib_paths size is", lib_paths.size());

  for (auto &lib_path : lib_paths) {
    std::string lib_src;

    try {
      std::ifstream fs(lib_path);

      // Read everything into str
      lib_src = std::string((std::istreambuf_iterator<char>(fs)),
          std::istreambuf_iterator<char>());

    } catch (std::ifstream::failure e) {
      throw std::runtime_error("Couldn't open file for fragment shader lib");
    }

    program_src.insert(second_line_loc + 1, lib_src);

  }

  log_info("Compiled fragment shader code is", program_src);

  const char *c_str = program_src.c_str();
  return FragmentShader(&c_str);
}
