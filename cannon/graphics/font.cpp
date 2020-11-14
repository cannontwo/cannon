#include <cannon/graphics/font.hpp>

using namespace cannon::graphics;

void Font::load_chars() {
  for (unsigned char c = 0; c < 128; c++) {
    if (FT_Load_Char(face_, c, FT_LOAD_RENDER)) {
      std::runtime_error("Failed to load glyph");
    }    

    Vector2f size{face_->glyph->bitmap.width, face_->glyph->bitmap.rows};
    Vector2f bearing{face_->glyph->bitmap_left, face_->glyph->bitmap_top};
    auto t = std::make_shared<Texture>(face_, GL_TEXTURE0);

    chars_.emplace(c, std::make_shared<Character>(c, t, size, bearing, face_->glyph->advance.x));
  }
}

void Font::init() {
  // Disable word-alignment 
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  if (FT_Init_FreeType(&ft_)) {
    std::runtime_error("Could not init FreeType library");
  }
  log_info("Initialized library");
  log_info(ft_);


  if (FT_New_Face(ft_, path_.c_str(), 0, &face_)) {
    std::runtime_error("Could not load font");
  }
  log_info("Initialized face");
  log_info(face_);

  FT_Set_Pixel_Sizes(face_, 0, size_);

  // Load 128 ASCII characters and generate textures
  load_chars();

  initialized_ = true;
}

std::shared_ptr<Character> Font::get_char(char c) {
  if (!initialized_)
    throw std::runtime_error("Font must be initialized before use");


  return chars_[c];
}
