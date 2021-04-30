#include <iostream>
#include <string>
#include <stdexcept>

#include <yaml-cpp/yaml.h>
#include <Eigen/Dense>

#include <cannon/math/random_double.hpp>
#include <cannon/ray/ray.hpp>
#include <cannon/ray/write_ppm.hpp>
#include <cannon/ray/hittable_list.hpp>
#include <cannon/ray/sphere.hpp>
#include <cannon/ray/moving_sphere.hpp>
#include <cannon/ray/camera.hpp>
#include <cannon/ray/material.hpp>
#include <cannon/ray/raytracer.hpp>
#include <cannon/graphics/random_color.hpp>
#include <cannon/log/registry.hpp>

using namespace Eigen;

using namespace cannon::ray;
using namespace cannon::math;
using namespace cannon::log;

// TODO Allow loading of world from YAML file
std::shared_ptr<HittableList> random_scene() {
  auto world = std::make_shared<HittableList>();

  auto ground_material = std::make_shared<Lambertian>(Vector3d(0.5, 0.5, 0.5));
  world->add(std::make_shared<Sphere>(Vector3d(0, -1000, 0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();

      Vector3d center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

      if ((center - Vector3d(4, 0.2, 0)).norm() > 0.9) {
        auto albedo = (Vector3d::Random().array() * Vector3d::Random().array()).matrix();

        if (choose_mat < 0.8) {
          // Diffuse
          auto sphere_material = std::make_shared<Lambertian>(albedo);
          Vector3d center2 = center + Vector3d(0, random_double(0, .5), 0);
          world->add(std::make_shared<MovingSphere>(center, center2, 0.0, 1.0, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // Metal
          auto fuzz = random_double(0, 0.5);
          auto sphere_material = std::make_shared<Metal>(albedo, fuzz);
          world->add(std::make_shared<Sphere>(center, 0.2, sphere_material));
        } else {
          // Glass
          auto sphere_material = std::make_shared<Dielectric>(1.5);
          world->add(std::make_shared<Sphere>(center, 0.2, sphere_material));
        }
      }

    }
  }

  auto material1 = std::make_shared<Dielectric>(1.5);
  world->add(std::make_shared<Sphere>(Vector3d(0, 1, 0), 1.0, material1));

  auto material2 = std::make_shared<Lambertian>(Vector3d(0.4, 0.2, 0.1));
  world->add(std::make_shared<Sphere>(Vector3d(-4, 1, 0), 1.0, material2));

  auto material3 = std::make_shared<Metal>(Vector3d(0.7, 0.6, 0.5), 0.0);
  world->add(std::make_shared<Sphere>(Vector3d(4, 1, 0), 1.0, material3));

  return world;
}

int main(int argc, char** argv) {
  if (argc <= 1) {
    log_error("Provide raytracer config file as argument");
    return 1;
  }
  
  // World
  auto world = random_scene();

  // Raytracer
  Raytracer raytracer(argv[1], world);
  //raytracer.render(std::cout);
  raytracer.render("test.ppm");
}
