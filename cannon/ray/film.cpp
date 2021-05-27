#include <cannon/ray/film.hpp>

#include <iostream>
#include <fstream>

using namespace cannon::ray;

std::unique_ptr<FilmTile> Film::get_film_tile(int i, int j) const {
  auto tile = std::make_unique<FilmTile>();

  tile->origin_x_ = i * tile_size_;
  tile->extent_x_ = tile_size_;
  if ((tile->origin_x_ + tile->extent_x_) >= width_)
    tile->extent_x_ = width_ - tile->origin_x_;

  tile->origin_y_ = j * tile_size_;
  tile->extent_y_ = tile_size_;
  if ((tile->origin_y_ + tile->extent_y_) >= height_)
    tile->extent_y_ = height_ - tile->origin_y_;

  tile->pixels_ = std::vector<FilmPixel>(tile->extent_x_ * tile->extent_y_);

  return tile;
}

void Film::merge_film_tile(std::unique_ptr<FilmTile> tile) {
  std::lock_guard<std::mutex> lock(mut_);

  for (unsigned int i = 0; i < tile->extent_x_; i++) {
    for (unsigned int j = 0; j < tile->extent_y_; j++) {
      int pixel_x = tile->origin_x_ + i;
      int pixel_y = tile->origin_y_ + j;

      pixels_[(height_ - pixel_y - 1) * width_ + pixel_x].color_sum_ += tile->pixels_[j * tile->extent_x_ + i].color_sum_;
      pixels_[(height_ - pixel_y - 1) * width_ + pixel_x].filter_weight_sum_ += tile->pixels_[j * tile->extent_x_ + i].filter_weight_sum_;
    }
  }
}

void Film::write_image(const std::string& filename, int samples) {
  std::lock_guard<std::mutex> lock(mut_);
  std::ofstream image_file(filename);

  image_file << "P3\n" << width_ << ' ' << height_ << "\n255\n";
  write_colors(image_file, pixels_);
  image_file.flush();
}

