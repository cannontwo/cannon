#include <iostream>

#include <Eigen/Dense>

#include <cannon/math/random_double.hpp>
#include <cannon/ray/ray.hpp>
#include <cannon/ray/write_ppm.hpp>
#include <cannon/ray/hittable_list.hpp>
#include <cannon/ray/sphere.hpp>
#include <cannon/ray/camera.hpp>
#include <cannon/ray/material.hpp>
#include <cannon/graphics/random_color.hpp>

using namespace Eigen;

using namespace cannon::ray;
using namespace cannon::math;

inline double clamp(double x, double min, double max) {
  return std::min(max, std::max(min, x));
}

// Background gradient
Vector3d ray_color(const Ray& r, const Hittable& world, int depth) {
  hit_record rec;

  if (depth <= 0)
    return Vector3d::Zero();

  if (world.hit(r, 0.001, std::numeric_limits<double>::infinity(), rec)) {
    // Lambertian diffuse texturing
    //Vector3d target = rec.p + rec.normal + random_unit_vec();

    //Lambertian diffuse texturing with hemispheric scattering
    //Vector3d target = rec.p + random_in_hemisphere(rec.normal);
    
    Ray scattered;
    Vector3d attenuation = Vector3d::Zero();
    if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
      return (attenuation.array() * ray_color(scattered, world, depth-1).array()).matrix();
    } else {
      return Vector3d::Zero();
    }
  } else {
    Vector3d blue; 
    blue << 0.5,
            0.7,
            1.0;

    Vector3d unit_direction = r.dir_.normalized();

    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Vector3d::Ones() + t * blue;
  }
}

HittableList random_scene() {
  HittableList world;

  auto ground_material = std::make_shared<Lambertian>(Vector3d(0.5, 0.5, 0.5));
  world.add(std::make_shared<Sphere>(Vector3d(0, -1000, 0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();

      Vector3d center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

      if ((center - Vector3d(4, 0.2, 0)).norm() > 0.9) {
        auto albedo = (Vector3d::Random().array() * Vector3d::Random().array()).matrix();

        if (choose_mat < 0.8) {
          // Diffuse
          auto sphere_material = std::make_shared<Lambertian>(albedo);
          world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // Metal
          auto fuzz = random_double(0, 0.5);
          auto sphere_material = std::make_shared<Metal>(albedo, fuzz);
          world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
        } else {
          // Glass
          auto sphere_material = std::make_shared<Dielectric>(1.5);
          world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
        }
      }

    }
  }

  auto material1 = std::make_shared<Dielectric>(1.5);
  world.add(std::make_shared<Sphere>(Vector3d(0, 1, 0), 1.0, material1));

  auto material2 = std::make_shared<Lambertian>(Vector3d(0.4, 0.2, 0.1));
  world.add(std::make_shared<Sphere>(Vector3d(-4, 1, 0), 1.0, material2));

  auto material3 = std::make_shared<Metal>(Vector3d(0.7, 0.6, 0.5), 0.0);
  world.add(std::make_shared<Sphere>(Vector3d(4, 1, 0), 1.0, material3));

  return world;
}

int main() {

  // Image 
  const auto aspect_ratio = 3.0 / 2.0;
  const int image_width = 1200; 
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 50;
  const int max_depth = 50;
  
  // World
  HittableList world = random_scene();

  // Camera
  Vector3d look_from(13, 2, 3);
  Vector3d look_at(0, 0, 0);
  Vector3d vup(0, 1, 0);
  auto dist_to_focus = 10.0;
  auto aperture = 0.1;
  Camera camera(look_from, look_at, vup, M_PI / 8, aspect_ratio, aperture, dist_to_focus);

  // Render
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (int j = image_height - 1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining: " << j << " " << std::flush;
    for (int i = 0; i < image_width; ++i) {
      Vector3d pixel_color = Vector3d::Zero();

      for (int s = 0; s < samples_per_pixel; s++) {
        auto u = (i + random_double()) / (image_width - 1);
        auto v = (j + random_double()) / (image_height - 1);

        Ray r = camera.get_ray(u, v);
        pixel_color += ray_color(r, world, max_depth);
      }

      write_color(std::cout, pixel_color, samples_per_pixel);
    }
  }

  std::cerr << "\nDone.\n";
}
