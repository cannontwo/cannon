#include <cannon/graphics/opengl_writer.hpp>

#include <stb_image/stb_image_write.h>

using namespace cannon::graphics;

std::string OpenGLWriter::getFileExtensionWithDot() const {
  return std::string(".png");
}

void OpenGLWriter::write(std::string path) const {
  stbi_flip_vertically_on_write(true);
  if (stbi_write_png(path.c_str(), data_.width, data_.height,
        data_.num_channels, data_.buffer->data(), data_.stride) == 0) {
    throw std::runtime_error("Could not write OpenGL render to file"); 
  }
}

void OpenGLWriter::cleanUpReceived(std::string /*receivedPath*/) const {
  close_func_();
}
