#include <cannon/ray/film.hpp>

#include <iostream>
#include <fstream>
#include <cmath>

#include <cannon/ray/filter.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::ray;
using namespace cannon::log;

void FilmTile::add_sample(const Vector2d& p_film, const Vector3d& color) {
  // Compute sample raster bounds
  Vector2d p_film_discrete = p_film - Vector2d(0.5, 0.5);
  Vector2i p0 = (p_film_discrete - filter_radius_).array().ceil().matrix().cast<int>();
  Vector2i p1 = (p_film_discrete + filter_radius_).array().ceil().matrix().cast<int>() + Vector2i(1, 1);
  
  p0 = p0.cwiseMax(Vector2i(origin_x_, origin_y_));
  p1 = p1.cwiseMin(Vector2i(origin_x_ + extent_x_, origin_y_ + extent_y_));

  // Add sample to all pixels its raster overlaps
  for (int j = p0.y(); j < p1.y(); j++) {
    for (int i = p0.x(); i < p1.x(); i++) {
      Vector2d offset(i - p_film_discrete.x(), j - p_film_discrete.y());
      offset[0] = std::abs(offset.x() * inv_filter_radius_.x() * filter_table_width_);
      offset[1] = std::abs(offset.y() * inv_filter_radius_.y() * filter_table_width_);

      int arr_offset_x = std::min((int)std::floor(offset[0]), filter_table_width_ - 1);
      int arr_offset_y = std::min((int)std::floor(offset[1]), filter_table_width_ - 1);

      double filter_weight = filter_table_[arr_offset_y * filter_table_width_ + arr_offset_x];

      FilmPixel& pixel = get_pixel(i, j);
      pixel.color_sum_ += color * filter_weight;
      pixel.filter_weight_sum_ += filter_weight;
    }
  }
}

FilmPixel& FilmTile::get_pixel(int i, int j) {
  return pixels_[(j - origin_y_) * extent_x_ + (i - origin_x_)];
}

Film::Film(unsigned int width, unsigned int height, unsigned int tile_size,
           std::unique_ptr<Filter> filter)
    : width_(width), height_(height), tile_size_(tile_size),
      pixels_(width * height), filter_(std::move(filter)) {

  // Precompute cached filter weights
  int offset = 0;
  for (int j = 0; j < filter_table_width_; j++) {
    for (int i = 0; i < filter_table_width_; i++, offset++) {
      Vector2d p;
      p[0] = (i + 0.5) * filter_->radius_.x() / filter_table_width_;
      p[1] = (j + 0.5) * filter_->radius_.y() / filter_table_width_;
      filter_table_[offset] = filter_->evaluate(p);
    }
  }
}

std::unique_ptr<FilmTile> Film::get_film_tile(int i, int j) const {
  auto tile = std::make_unique<FilmTile>();

  // Extend tile to include filter sampling radius
  tile->origin_x_ = std::max(0.0, std::ceil(i * tile_size_ - 0.5 - filter_->radius_.x()));
  tile->extent_x_ = tile_size_ + (2 * filter_->radius_.x());
  if ((tile->origin_x_ + tile->extent_x_) >= width_)
    tile->extent_x_ = width_ - tile->origin_x_;

  tile->origin_y_ = std::max(0.0, std::ceil(j * tile_size_ - 0.5 - filter_->radius_.y()));
  tile->extent_y_ = tile_size_ + (2 * filter_->radius_.y());
  if ((tile->origin_y_ + tile->extent_y_) >= height_)
    tile->extent_y_ = height_ - tile->origin_y_;

  tile->filter_radius_ = filter_->radius_;
  tile->inv_filter_radius_ = filter_->inv_radius_;
  tile->filter_table_ = filter_table_;
  tile->filter_table_width_ = filter_table_width_;

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

void Film::write_image(const std::string& filename) {
  std::lock_guard<std::mutex> lock(mut_);
  std::ofstream image_file(filename);

  image_file << "P3\n" << width_ << ' ' << height_ << "\n255\n";
  write_colors(image_file, pixels_);
  image_file.flush();
}

void Film::write_image(float *data) {
  std::lock_guard<std::mutex> lock(mut_);

  for (unsigned int i = 0; i < width_; i++) {
    for (unsigned int j = 0; j < height_; j++) {
      int offset = 3 * ((width_ - i) * height_  + j);

      Vector3d normalized_color = pixels_[i * height_ + j].color_sum_ /
                                  pixels_[i * height_ + j].filter_weight_sum_;

      // Correct for gamma=2.0
      data[offset] = std::min(0.999f, std::max(0.f, std::sqrt((float)normalized_color.x())));
      data[offset+1] = std::min(0.999f, std::max(0.f, std::sqrt((float)normalized_color.y())));
      data[offset+2] = std::min(0.999f, std::max(0.f, std::sqrt((float)normalized_color.z())));
    }
  }


}
