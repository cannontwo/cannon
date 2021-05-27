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
#include <cannon/ray/bvh.hpp>
#include <cannon/ray/aa_rect.hpp>
#include <cannon/ray/constant_medium.hpp>
#include <cannon/ray/mesh.hpp>
#include <cannon/ray/texture.hpp>
#include <cannon/ray/filter.hpp>
#include <cannon/graphics/random_color.hpp>
#include <cannon/log/registry.hpp>
#include <cannon/utils/statistics.hpp>

using namespace Eigen;

using namespace cannon::ray;
using namespace cannon::math;
using namespace cannon::log;
using namespace cannon::utils;

std::shared_ptr<HittableList> random_scene() {
  auto world = std::make_shared<HittableList>();

  auto checker = std::make_shared<CheckerTexture>(Vector3d(0.2, 0.3, 0.1), Vector3d(0.9, 0.9, 0.9));
  world->add(std::make_shared<Sphere>(Vector3d(0, -1000, 0), 1000, std::make_shared<Lambertian>(checker)));

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

std::shared_ptr<HittableList> two_spheres_scene() {
  auto objects = std::make_shared<HittableList>();
  auto checker = std::make_shared<CheckerTexture>(Vector3d(0.2, 0.3, 0.1), Vector3d(0.9, 0.9, 0.9));

  objects->add(std::make_shared<Sphere>(Vector3d(0, -10, 0), 10, std::make_shared<Lambertian>(checker)));
  objects->add(std::make_shared<Sphere>(Vector3d(0, 10, 0), 10, std::make_shared<Lambertian>(checker)));

  return objects;
}

std::shared_ptr<HittableList> two_perlin_spheres_scene() {
  auto objects = std::make_shared<HittableList>();

  auto pertext = std::make_shared<NoiseTexture>(4);

  objects->add(std::make_shared<Sphere>(Vector3d(0, -1000, 0), 1000, std::make_shared<Lambertian>(pertext)));
  objects->add(std::make_shared<Sphere>(Vector3d(0, 2, 0), 2, std::make_shared<Lambertian>(pertext)));

  return objects;
}

std::shared_ptr<HittableList> earth_scene() {
  auto world = std::make_shared<HittableList>();

  auto earth_texture = std::make_shared<ImageTexture>("assets/earthmap.jpg");
  auto earth_surface = std::make_shared<Lambertian>(earth_texture);
  auto globe = std::make_shared<Sphere>(Vector3d(0, 2, 0), 2, earth_surface);

  world->add(globe);

  auto pertext = std::make_shared<NoiseTexture>(4);
  world->add(std::make_shared<Sphere>(Vector3d(0, -1000, 0), 1000, std::make_shared<Lambertian>(pertext)));

  return world;
}

std::shared_ptr<HittableList> simple_light_scene() {
  auto world = std::make_shared<HittableList>();

  auto pertext = std::make_shared<NoiseTexture>(4);
  world->add(std::make_shared<Sphere>(Vector3d(0, -1000, 0), 1000, std::make_shared<Lambertian>(pertext)));
  world->add(std::make_shared<Sphere>(Vector3d(0, 2, 0), 2, std::make_shared<Lambertian>(pertext)));

  // Light brighter than (1, 1, 1) for lighting
  auto difflight = std::make_shared<DiffuseLight>(Vector3d(4, 4, 4));
  world->add(std::make_shared<XYRect>(3, 5, 1, 3, -2, difflight));
  world->add(std::make_shared<Sphere>(Vector3d(0, 5, 0), 1, difflight));

  return world;
}

std::shared_ptr<HittableList> cornell_box() {
  auto world = std::make_shared<HittableList>();

  auto red = std::make_shared<Lambertian>(Vector3d(0.65, 0.05, 0.05));
  auto white = std::make_shared<Lambertian>(Vector3d(0.73, 0.73, 0.73));
  auto green = std::make_shared<Lambertian>(Vector3d(0.12, 0.45, 0.15));
  auto light = std::make_shared<DiffuseLight>(Vector3d(15, 15, 15));

  world->add(std::make_shared<YZRect>(0, 555, 0, 555, 555, green));
  world->add(std::make_shared<YZRect>(0, 555, 0, 555, 0, red));
  world->add(std::make_shared<XZRect>(213, 343, 227, 332, 554, light));
  world->add(std::make_shared<XZRect>(0, 555, 0, 555, 0, white));
  world->add(std::make_shared<XZRect>(0, 555, 0, 555, 555, white));
  world->add(std::make_shared<XYRect>(0, 555, 0, 555, 555, white));

  auto t = std::make_shared<Affine3d>(Affine3d::Identity());
  t->translate(Vector3d(265, 0, 295));
  t->rotate(AngleAxisd(0.2618, Vector3d::UnitY()));
  std::shared_ptr<Hittable> box1 = std::make_shared<Box>(t, Vector3d(0, 0, 0),
      Vector3d(165, 330, 165), white);
  world->add(box1);

  auto t2 = std::make_shared<Affine3d>(Affine3d::Identity());
  t2->translate(Vector3d(130, 0, 65));
  t2->rotate(AngleAxisd(-0.31415, Vector3d::UnitY()));
  std::shared_ptr<Hittable> box2 = std::make_shared<Box>(t2, Vector3d(0, 0, 0),
      Vector3d(165, 165, 165), white);
  world->add(box2);

  return world;
}

std::shared_ptr<HittableList> final_scene() {
  auto world = std::make_shared<HittableList>();
  auto ground = std::make_shared<Lambertian>(Vector3d(0.48, 0.83, 0.53));

  auto trans = std::make_shared<Affine3d>(Affine3d::Identity());

  const int boxes_per_side = 20;
  for (int i = 0; i < boxes_per_side; i++) {
    for (int j = 0; j < boxes_per_side; j++) {
      auto w = 100.0;
      auto x0 = -1000.0 + i*w;
      auto z0 = -1000.0 + j*w;
      auto y0 = 0.0;
      auto x1 = x0 + w;
      auto y1 = random_double(1, 101);
      auto z1 = z0 + w;

      world->add(std::make_shared<Box>(trans, Vector3d(x0, y0, z0), Vector3d(x1, y1, z1), ground));
    }
  } 

  auto light = std::make_shared<DiffuseLight>(Vector3d(7, 7, 7));
  world->add(std::make_shared<XZRect>(123, 423, 147, 412, 554, light));

  Vector3d center1(400, 400, 200);
  Vector3d center2 = center1 + Vector3d(30, 0, 0);
  auto moving_sphere_material = std::make_shared<Lambertian>(Vector3d(0.7, 0.3, 0.1));
  world->add(std::make_shared<MovingSphere>(center1, center2, 0, 1, 50, moving_sphere_material));

  world->add(std::make_shared<Sphere>(Vector3d(260, 150, 45), 50, std::make_shared<Dielectric>(1.5)));
  world->add(std::make_shared<Sphere>(Vector3d(0, 150, 145), 50,
        std::make_shared<Metal>(Vector3d(0.8, 0.8, 0.9), 1.0)));

  auto boundary = std::make_shared<Sphere>(Vector3d(360, 150, 145), 70, std::make_shared<Dielectric>(1.5));
  world->add(boundary);
  world->add(std::make_shared<ConstantMedium>(boundary, 0.2, Vector3d(0.2, 0.4, 0.9)));
  boundary = std::make_shared<Sphere>(Vector3d(0, 0, 0), 5000, std::make_shared<Dielectric>(1.5));
  world->add(std::make_shared<ConstantMedium>(boundary, .0001, Vector3d(1, 1, 1)));

  auto emat = std::make_shared<Lambertian>(std::make_shared<ImageTexture>("assets/earthmap.jpg"));
  world->add(std::make_shared<Sphere>(Vector3d(400, 200, 400), 100, emat));
  auto pertext = std::make_shared<NoiseTexture>(0.1);
  world->add(std::make_shared<Sphere>(Vector3d(220, 280, 300), 80, std::make_shared<Lambertian>(pertext)));

  auto boxes = std::make_shared<HittableList>();
  auto white = std::make_shared<Lambertian>(Vector3d(0.73, 0.73, 0.73));
  int ns = 1000;
  for (int j = 0; j < ns; j++) {
    boxes->add(std::make_shared<Sphere>(random_vec(0, 165), 10, white));
  }

  auto t = std::make_shared<Affine3d>(Affine3d::Identity());
  t->translate(Vector3d(-100, 270, 395));
  t->rotate(AngleAxisd(0.2618, Vector3d::UnitY()));

  world->add(std::make_shared<BvhNode>(t, boxes, 0.0, 1.0));

  return world;
}

std::shared_ptr<HittableList> model_test() {
  auto world = std::make_shared<HittableList>();

  //auto c = std::make_shared<NormalDebug>();
  auto c = std::make_shared<Lambertian>(Vector3d(0.48, 0.83, 0.53));

  auto t = std::make_shared<Affine3d>(Affine3d::Identity());
  std::vector<std::shared_ptr<TriangleMesh>> model = load_model(t, c, "assets/backpack/backpack.obj");
  //std::vector<std::shared_ptr<TriangleMesh>> model = load_model(t, c, "assets/sphere/sphere.obj");
  for (auto& mesh : model) {
    world->add(std::make_shared<BvhNode>(std::make_shared<Affine3d>(Affine3d::Identity()), make_mesh_triangle_list(mesh), 0.0, 1.0));
  }

  auto pertext = std::make_shared<NoiseTexture>(4);
  world->add(std::make_shared<Sphere>(Vector3d(0, -1002, 0), 1000, std::make_shared<Lambertian>(pertext)));

  return world;
}

int main(int argc, char** argv) {
  if (argc <= 1) {
    log_error("Provide raytracer config file as argument");
    return 1;
  }

  // World
  //auto world = random_scene();
  //auto world = two_spheres_scene();
  //auto world = two_perlin_spheres_scene();
  //auto world = earth_scene();
  //auto world = simple_light_scene();
  auto world = cornell_box();
  //auto world = final_scene();
  //auto world = model_test();

  auto t = std::make_shared<Affine3d>(Affine3d::Identity());
  //log_info("Building bounding volume hierarchy");
  auto bvh = std::make_shared<BvhNode>(t, world, 0.0, 1.0);

  // Raytracer
  //log_info("Rendering");
  Raytracer raytracer(argv[1], bvh);
  //raytracer.render(std::cout);
  raytracer.render("test.ppm", std::make_unique<GaussianFilter>(Vector2d::Ones() * 2.0, 1.0));
  //raytracer.render("test.ppm", std::make_unique<MitchellFilter>(Vector2d::Ones() * 2.0, 1.0/3.0, 1.0/3.0));
  
  // Report and write stats out
  report_thread_stats();
  print_stats("raytracer_stats.txt");
}
