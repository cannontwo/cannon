#pragma once
#ifndef CANNON_RAY_RAYTRACER_H
#define CANNON_RAY_RAYTRACER_H

/*!
 * \file cannon/ray/raytracer.hpp
 * File containing raytracer_params definition and Raytracer class definition.
 */

#include <iostream>
#include <fstream>

#include <Eigen/Dense>
#include <yaml-cpp/yaml.h>

#include <cannon/math/random_double.hpp>
#include <cannon/ray/ray.hpp>
#include <cannon/ray/write_ppm.hpp>
#include <cannon/ray/hittable.hpp>
#include <cannon/ray/sphere.hpp>
#include <cannon/ray/camera.hpp>
#include <cannon/ray/material.hpp>
#include <cannon/utils/thread_pool.hpp>

using namespace Eigen;

using namespace cannon::math;
using namespace cannon::utils;

namespace cannon {
  namespace ray { 

    /*!
     * \brief Struct containing Raytracer params that can be read from YAML config.
     */
    struct raytracer_params {
      double aspect_ratio;
      int image_width; 
      int image_height;

      int samples_per_pixel;
      int max_depth;
      double dist_to_focus;
      double aperture;
      double vfov;

      Vector3d look_from = Vector3d::Zero();
      Vector3d look_at = Vector3d::Zero();
      Vector3d vup = Vector3d::Zero();

      Vector3d background_color = Vector3d::Zero();
    };

    /*!
     * \brief Class representing core raytracer functionality.
     */
    class Raytracer {
      public:
        Raytracer() = delete;

        /*!
         * Constructor taking raytracer config filename and world geometry.
         */
        Raytracer(const std::string& config_filename, std::shared_ptr<Hittable>
            world) : params_(load_config(config_filename)), world_(world),
        camera_(params_.look_from, params_.look_at, params_.vup, params_.vfov,
            params_.aspect_ratio, params_.aperture, params_.dist_to_focus) {}

        /*!
         * Load raytracer params from YAML file.
         *
         * \param filename Filename to load params from.
         */
        raytracer_params load_config(const std::string& filename);

        /*!
         * Render scene via raytracing to the input stream.
         *
         * \param os The stream to render to.
         */
        void render(std::ostream& os);

        /*!
         * Render scene progressively, sample by sample, to the input file.
         *
         * \param out_filename File to write rendered image to.
         */
        void render(const std::string& out_filename);

      private:
        template <typename T>
        T safe_get_param_(YAML::Node config, const std::string& param) {
          if (!config[param]) {
            throw std::runtime_error(std::string("Could not get param ") + param + " in YAML config.");
          } else {
            return config[param].as<T>();
          }
        }

        /*!
         * Method that performs the actual raycasting into the scene.
         *
         * \param r Ray into the scene.
         * \param depth Child ray depth limiting infinite recursion.
         */
        Vector3d ray_color(const Ray& r, int depth);

        raytracer_params params_; //!< Rendering parameters
        std::shared_ptr<Hittable> world_; //!< World geometry
        Camera camera_; //!< Rendering camera
        Vector3d background_; //!< Background color for rendering

    };

  } // namespace ray
} // namespace cannon

#endif /* ifndef CANNON_RAY_RAYTRACER_H */
