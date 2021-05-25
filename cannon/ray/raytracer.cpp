#include <cannon/ray/raytracer.hpp>

#include <cannon/ray/hittable.hpp>
#include <cannon/ray/ray.hpp>
#include <cannon/ray/sphere.hpp>
#include <cannon/ray/write_ppm.hpp>
#include <cannon/ray/film.hpp>
#include <cannon/ray/material.hpp>
#include <cannon/utils/thread_pool.hpp>
#include <cannon/utils/statistics.hpp>
#include <cannon/math/random_double.hpp>

using namespace cannon::ray;
using namespace cannon::math;
using namespace cannon::utils;

raytracer_params Raytracer::load_config(const std::string& filename) {
  raytracer_params params;
  YAML::Node config = YAML::LoadFile(filename);

  params.aspect_ratio = safe_get_param_<double>(config, "aspect_ratio");
  params.image_width = safe_get_param_<int>(config, "image_width");
  params.image_height = static_cast<int>(params.image_width / params.aspect_ratio);

  params.samples_per_pixel = safe_get_param_<int>(config, "samples_per_pixel");
  params.max_depth = safe_get_param_<int>(config, "max_depth");
  params.dist_to_focus = safe_get_param_<double>(config, "dist_to_focus");
  params.aperture = safe_get_param_<double>(config, "aperture");
  params.vfov = safe_get_param_<double>(config, "vfov");

  if (!config["look_from"])
    throw new std::runtime_error("Could not find look_from map");

  YAML::Node look_from_params = config["look_from"];
  params.look_from[0] = safe_get_param_<double>(look_from_params, "x");
  params.look_from[1] = safe_get_param_<double>(look_from_params, "y");
  params.look_from[2] = safe_get_param_<double>(look_from_params, "z");

  if (!config["look_at"])
    throw new std::runtime_error("Could not find look_at map");

  YAML::Node look_at_params = config["look_at"];
  params.look_at[0] = safe_get_param_<double>(look_at_params, "x");
  params.look_at[1] = safe_get_param_<double>(look_at_params, "y");
  params.look_at[2] = safe_get_param_<double>(look_at_params, "z");

  if (!config["vup"])
    throw new std::runtime_error("Could not find vup map");

  YAML::Node vup_params = config["vup"];
  params.vup[0] = safe_get_param_<double>(vup_params, "x");
  params.vup[1] = safe_get_param_<double>(vup_params, "y");
  params.vup[2] = safe_get_param_<double>(vup_params, "z");

  if (!config["background_color"])
    throw new std::runtime_error("Could not find background_color map");

  YAML::Node background_params = config["background_color"];
  params.background_color[0] = safe_get_param_<double>(background_params, "x");
  params.background_color[1] = safe_get_param_<double>(background_params, "y");
  params.background_color[2] = safe_get_param_<double>(background_params, "z");

  return params;
}

Vector3d Raytracer::ray_color(const Ray& r, int depth) {
  hit_record rec;

  if (depth <= 0)
    return Vector3d::Zero();

  if (!world_->hit(r, 0.001, std::numeric_limits<double>::infinity(), rec))
    return params_.background_color;

  Ray scattered;
  Vector3d attenuation = Vector3d::Zero();
  Vector3d emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

  if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
    return emitted;

  return emitted + (attenuation.array() * ray_color(scattered, depth-1).array()).matrix();
}

void Raytracer::render(std::ostream& os) {
  os << "P3\n" << params_.image_width << ' ' << params_.image_height << "\n255\n";

  for (int j = params_.image_height - 1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining: " << j << " " << std::flush;
    for (int i = 0; i < params_.image_width; ++i) {
      Vector3d pixel_color = Vector3d::Zero();

      for (int s = 0; s < params_.samples_per_pixel; s++) {
        auto u = (i + random_double()) / (params_.image_width - 1);
        auto v = (j + random_double()) / (params_.image_height - 1);

        Ray r = camera_.get_ray(u, v);
        pixel_color += ray_color(r, params_.max_depth);
      }

      write_color(os, pixel_color, params_.samples_per_pixel);
    }
  }

  std::cerr << "\nDone.\n";
}

void Raytracer::render(const std::string& out_filename, int tile_size) {
  Film film(params_.image_width, params_.image_height, tile_size);

  // TODO This is fantastically slower than the single-threaded version somehow, and I have no idea why. Memory allocation?
  ThreadPool<std::pair<int, int>> pool([&](std::shared_ptr<std::pair<int, int>> tile_coord) {
      auto tile = film.get_film_tile(tile_coord->first, tile_coord->second);

      for (unsigned int i = 0; i < tile->extent_x_; i++) {
        for (unsigned int j = 0; j < tile->extent_y_; j++) {
          for (int s = 0; s < params_.samples_per_pixel; s++) {
            auto u = (i + tile->origin_x_ + random_double()) / (params_.image_width - 1);
            auto v = (j + tile->origin_y_ + random_double()) / (params_.image_height - 1);

            Ray r = camera_.get_ray(u, v);
            Vector3d pixel_color = ray_color(r, params_.max_depth);

            tile->data_.col(j * tile_size + i) += pixel_color;
          }
        }
      }

      std::cerr << "\rFinished tile (" << tile_coord->first << ", " << tile_coord->second << ")" << std::flush;

      film.merge_film_tile(std::move(tile));
      
      report_thread_stats();
      });

  // Enqueue work, one item per sample. Could alternatively be done by chunking portions of image
  for (int i = 0; i < std::floor(params_.image_width / tile_size); i++) {
    for (int j = 0; j < std::floor(params_.image_height / tile_size); j++) {
      pool.enqueue(std::make_shared<std::pair<int, int>>(std::make_pair(i, j)));
    }
  }

  pool.join();

  film.write_image(out_filename, params_.samples_per_pixel);

}
