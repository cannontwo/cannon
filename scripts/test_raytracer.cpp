#include <iostream>

#include <Eigen/Dense>

#include <cannon/math/random_double.hpp>
#include <cannon/ray/ray.hpp>
#include <cannon/ray/write_ppm.hpp>
#include <cannon/ray/hittable_list.hpp>
#include <cannon/ray/sphere.hpp>
#include <cannon/ray/camera.hpp>

using namespace Eigen;

using namespace cannon::ray;
using namespace cannon::math;

inline double clamp(double x, double min, double max) {
  return std::min(max, std::max(min, x));
}

// Sphere intersection
double hit_sphere(const Vector3d& center, double radius, const Ray& r) {
  Vector3d oc = r.orig_ - center;
  auto a = r.dir_.dot(r.dir_);
  auto b = oc.dot(r.dir_);
  auto c = oc.dot(oc) - radius * radius;
  auto discriminant = b*b - a*c;

  if (discriminant < 0) {
    return -1.0;
  } else {
    return (-b - std::sqrt(discriminant)) / a;
  }
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
    Vector3d target = rec.p + random_in_hemisphere(rec.normal);

    // Diffuse material computation via child rays
    return 0.5 * ray_color(Ray(rec.p, target - rec.p), world, depth - 1);
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

int main() {

  // Image 
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 400; 
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixel = 100;
  const int max_depth = 50;
  
  // World
  HittableList world;
  Vector3d sphere_center_1, sphere_center_2;
  sphere_center_1 << 0.0,
                     0.0,
                     -1.0;
  world.add(std::make_shared<Sphere>(sphere_center_1, 0.5));

  sphere_center_2 << 0.0,
                     -100.5,
                     -1;
  world.add(std::make_shared<Sphere>(sphere_center_2, 100));

  // Camera
  Camera camera;

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
