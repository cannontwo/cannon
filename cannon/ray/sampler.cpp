#include <cannon/ray/sampler.hpp>

#include <cannon/ray/camera.hpp>
#include <cannon/utils/statistics.hpp>
#include <cannon/math/random_double.hpp>

using namespace cannon::ray;
using namespace cannon::math;
using namespace cannon::utils;

Sampler::Sampler(unsigned int samples_per_pixel)
    : samples_per_pixel_(samples_per_pixel) {}

Sampler::~Sampler() {}

STAT_COUNTER("Sampler/Camera Samples", nCameraSamples);

CameraSample Sampler::get_camera_sample(const Vector2i &raster_point) {
  ++nCameraSamples;

  CameraSample cs;

  cs.p_film = raster_point.cast<double>() + get_2d();
  cs.p_lens = get_2d();
  cs.time = get_1d();

  return cs;
}

unsigned int Sampler::round_count(unsigned int n) const {
  return n;
}

void Sampler::start_pixel(const Vector2i& p) {
  current_pixel_ = p; 
  current_pixel_sample_idx_ = 0;

  array_1d_offset_ = array_2d_offset_ = 0;
}

bool Sampler::start_next_sample() {
  ++current_pixel_sample_idx_;
  array_1d_offset_ = array_2d_offset_ = 0;
  return current_pixel_sample_idx_ < samples_per_pixel_;
}

bool Sampler::set_sample_number(unsigned int sample_num) {
  current_pixel_sample_idx_ = sample_num;
  array_1d_offset_ = array_2d_offset_ = 0;
  return current_pixel_sample_idx_ < samples_per_pixel_;
}

void Sampler::request_1d_array(unsigned int n) {
  samples_1d_array_sizes_.push_back(n);
  sample_array_1d_.push_back(std::vector<double>(n * samples_per_pixel_)); 
}

void Sampler::request_2d_array(unsigned int n) {
  samples_2d_array_sizes_.push_back(n);
  sample_array_2d_.push_back(std::vector<Vector2d>(n * samples_per_pixel_)); 
}

const double* Sampler::get_1d_array(unsigned int n) {
  if (array_1d_offset_ == sample_array_1d_.size())
    return nullptr;
  return &sample_array_1d_[array_1d_offset_++][current_pixel_sample_idx_ * n];
}

const Vector2d* Sampler::get_2d_array(unsigned int n) {
  if (array_2d_offset_ == sample_array_2d_.size())
    return nullptr;
  return &sample_array_2d_[array_2d_offset_++][current_pixel_sample_idx_ * n];
}

PixelSampler::PixelSampler(unsigned int samples_per_pixel,
                           unsigned int sampled_dims)
    : Sampler(samples_per_pixel) {
  for (unsigned int i = 0; i < sampled_dims; ++i) {
    samples_1d_.push_back(std::vector<double>(samples_per_pixel_));
    samples_2d_.push_back(std::vector<Vector2d>(samples_per_pixel_));
  }
}

bool PixelSampler::start_next_sample() {
  current_1d_dim_ = current_2d_dim_ = 0;
  return Sampler::start_next_sample();
}

bool PixelSampler::set_sample_number(unsigned int sample_num) {
  current_1d_dim_ = current_2d_dim_ = 0;
  return Sampler::set_sample_number(sample_num);
}

double PixelSampler::get_1d() {
  if (current_1d_dim_ < samples_1d_.size()) {
    return samples_1d_[current_1d_dim_++][current_pixel_sample_idx_];
  } else {
    return random_double();
  }
}

Vector2d PixelSampler::get_2d() {
  if (current_2d_dim_ < samples_2d_.size()) {
    return samples_2d_[current_2d_dim_++][current_pixel_sample_idx_];
  } else {
    Vector2d ret_vec;
    ret_vec[0] = random_double();
    ret_vec[1] = random_double();
    return ret_vec;
  }
}

GlobalSampler::GlobalSampler(unsigned int samples_per_pixel)
    : Sampler(samples_per_pixel) {}

void GlobalSampler::start_pixel(const Vector2i& p) {
  Sampler::start_pixel(p);
  dimension_ = 0;
  interval_sample_index_ = get_index_for_sample(0);

  array_end_dim_ = array_start_dim_ + sample_array_1d_.size() + 2 * sample_array_2d_.size();

  for (size_t i = 0; i < samples_1d_array_sizes_.size(); ++i) {
    int n_samples = samples_1d_array_sizes_[i] * samples_per_pixel_;
    for (int j = 0; j < n_samples; ++j) {
      unsigned int index = get_index_for_sample(j);
      sample_array_1d_[i][j] = sample_dimension_(index, array_start_dim_ + i);
    }
  }

  unsigned int dim = array_start_dim_ + samples_1d_array_sizes_.size();
  for (size_t i = 0; i < samples_2d_array_sizes_.size(); ++i) {
    int n_samples = samples_2d_array_sizes_[i] * samples_per_pixel_;
    for (int j = 0; j < n_samples; ++j) {
      unsigned int index = get_index_for_sample(j);
      sample_array_2d_[i][j].x() = sample_dimension_(index, dim);
      sample_array_2d_[i][j].y() = sample_dimension_(index, dim);
    }
    dim += 2;
  }
}

bool GlobalSampler::start_next_sample() {
  dimension_ = 0;
  interval_sample_index_ = get_index_for_sample(current_pixel_sample_idx_ + 1);
  return Sampler::start_next_sample();
}

bool GlobalSampler::set_sample_number(unsigned int sample_num) {
  dimension_ = 0;
  interval_sample_index_ = get_index_for_sample(sample_num);
  return Sampler::set_sample_number(sample_num);
}

double GlobalSampler::get_1d() {
  if (dimension_ >= array_start_dim_ && dimension_ < array_end_dim_)
    dimension_ = array_end_dim_;

  return sample_dimension_(interval_sample_index_, dimension_++);
}

Vector2d GlobalSampler::get_2d() {
  if (dimension_ + 1 >= array_start_dim_ && dimension_ < array_end_dim_)
    dimension_ = array_end_dim_;

  Vector2d p(sample_dimension_(interval_sample_index_, dimension_),
             sample_dimension_(interval_sample_index_, dimension_ + 1));

  dimension_ += 2;
  return p;
}

StratifiedSampler::StratifiedSampler(unsigned int x_pixel_samples,
                                     unsigned int y_pixel_samples,
                                     bool jitter_samples,
                                     int n_sampled_dimensions)
    : PixelSampler(x_pixel_samples * y_pixel_samples, n_sampled_dimensions),
      x_pixel_samples_(x_pixel_samples), y_pixel_samples_(y_pixel_samples),
      jitter_samples_(jitter_samples) {}


STAT_COUNTER("Sampler/Stratified Sampler pixels", nStratifiedPixels);

void StratifiedSampler::start_pixel(const Vector2i& p) {
  ++nStratifiedPixels;

  // Generate single samples
  for (size_t i = 0; i < samples_1d_.size(); ++i) {
    stratified_sample_1d_(&samples_1d_[i][0], x_pixel_samples_ * y_pixel_samples_, jitter_samples_);
    shuffle_(&samples_1d_[i][0], x_pixel_samples_ * y_pixel_samples_, 1);
  }

  for (size_t i = 0; i < samples_2d_.size(); ++i) {
    stratified_sample_2d_(samples_2d_[i], jitter_samples_);
    shuffle_(&samples_2d_[i][0], x_pixel_samples_ * y_pixel_samples_, 1);
  }
  
  // Generate array samples 
  for (size_t i = 0; i < samples_1d_array_sizes_.size(); ++i) {
    for (unsigned int j = 0; j < samples_per_pixel_; ++j) {
      unsigned int count = samples_1d_array_sizes_[i];
      stratified_sample_1d_(&sample_array_1d_[i][j * count], count, jitter_samples_);
      shuffle_(&sample_array_1d_[i][j * count], count, 1);
    }
  }

  for (size_t i = 0; i < samples_2d_array_sizes_.size(); ++i) {
    for (unsigned int j = 0; j < samples_per_pixel_; ++j) {
      unsigned int count = samples_2d_array_sizes_[i];
      std::vector<double> samples(2*count);
      latin_hypercube_(samples, count, 2);

      for (unsigned int k = 0; k < count; ++k) {
        sample_array_2d_[i][j * count + k].x() = samples[2*k];
        sample_array_2d_[i][j * count + k].y() = samples[2*k + 1];
      }
    }
  }
  
  PixelSampler::start_pixel(p);
}

void StratifiedSampler::stratified_sample_1d_(double *sample_vec, unsigned int n_samples, bool jitter) {
  double inv_n_samples = 1.0 / static_cast<double>(n_samples);

  for (size_t i = 0; i < n_samples; ++i) {
    double delta = jitter ? random_double() : 0.5;
    sample_vec[i] = std::min((i + delta) * inv_n_samples, ONE_MINUS_EPSILON);
  }
}

void StratifiedSampler::stratified_sample_2d_(std::vector<Vector2d> &sample_vec, bool jitter) {
  double dx = 1.0 / static_cast<double>(x_pixel_samples_);
  double dy = 1.0 / static_cast<double>(y_pixel_samples_);

  unsigned int idx = 0;
  for (unsigned int y = 0; y < y_pixel_samples_; ++y) {
    for (unsigned int x = 0; x < x_pixel_samples_; ++x) {
      double jx = jitter ? random_double() : 0.5;
      double jy = jitter ? random_double() : 0.5;

      sample_vec[idx].x() = std::min((x + jx) * dx, ONE_MINUS_EPSILON);
      sample_vec[idx].y() = std::min((y + jy) * dy, ONE_MINUS_EPSILON);
      ++idx;
    }
  }
}

void StratifiedSampler::latin_hypercube_(std::vector<double> &sample_vec,
                                         unsigned int n_samples,
                                         unsigned int n_dim) {
  // Generate LHS samples along diagonal
  double inv_n_samples = 1.0 / static_cast<double>(n_samples);
  for (unsigned int i = 0; i < n_samples; ++i) {
    for (unsigned int j = 0; j < n_dim; ++j) {
      double sj = (i + random_double()) * inv_n_samples; 
      sample_vec[n_dim * i + j] = std::min(sj, ONE_MINUS_EPSILON);
    }
  }

  // Permute LHS samples in each dimension
  for (unsigned int i = 0; i < n_dim; ++i) {
    for (unsigned int j = 0; j < n_samples; ++j) {
      unsigned int other = j + std::floor(random_double(0.0, n_samples - j));
      std::swap(sample_vec[n_dim * j + i], sample_vec[n_dim * other + i]);
    }
  }
}
