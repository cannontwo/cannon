#pragma once
#ifndef CANNON_RAY_MATERIAL_H
#define CANNON_RAY_MATERIAL_H 

/*!
 * \file cannon/ray/material.hpp
 * File containing class definitions for Material and derived materials.
 */

#include <Eigen/Dense>

#include <cannon/ray/ray.hpp>
#include <cannon/ray/hittable.hpp>
#include <cannon/ray/texture.hpp>
#include <cannon/math/random_double.hpp>

using namespace Eigen;

using namespace cannon::math;

namespace cannon {
  namespace ray {

    // Forward declaration
    struct hit_record;

    /*!
     * \brief Abstract class representing a hittable geometry's material.
     */
    class Material {
      public:

        /*!
         * Method that scatters an incoming ray using the properties of the
         * specific material implementing this abstract class.
         *
         * \param r_in Incoming ray.
         * \param rec Hit record for the incoming ray, modified by this method.
         * \param attenuation Color of attenuated child ray.
         * \param scattered Scattered child ray.
         *
         * \returns Whether the input ray was scattered.
         */
        virtual bool scatter(const Ray& r_in, const hit_record& rec, Vector3d&
            attenuation, Ray& scattered) const = 0;
    };

    /*!
     * \brief Class representing a Lambertian diffuse material.
     */
    class Lambertian : public Material {
      public:

        /*!
         * Constructor taking albedo color.
         */
        Lambertian(const Vector3d& a) : albedo_(std::make_shared<SolidColor>(a)) {}

        /*!
         * Constructor taking albedo texture.
         */
        Lambertian(std::shared_ptr<Texture> a) : albedo_(a) {}

        /*!
         * Destructor.
         */
        virtual ~Lambertian() {}

        /*!
         * Inherited from Material.
         */
        virtual bool scatter(const Ray& r_in, const hit_record& rec, Vector3d&
            attenuation, Ray& scattered) const override;

      public:
        std::shared_ptr<Texture> albedo_; //!< Albedo color for this material.

    };

    /*!
     * \brief Class representing a simple reflective metal material.
     */
    class Metal : public Material {
      public:

        /*!
         * Constructor taking albedo and fuzziness.
         */
        Metal(const Vector3d& a, double f) : albedo_(a), fuzz_(f < 1 ? f : 1) {}

        /*!
         * Destructor.
         */
        virtual ~Metal() {}
        
        /*!
         * Inherited from Material.
         */
        virtual bool scatter(const Ray& r_in, const hit_record& rec, Vector3d&
            attenuation, Ray& scattered) const override;

      public:
        Vector3d albedo_; //!< Albedo color for this material.
        double fuzz_;
    };

    /*!
     * \brief Class representing a dielectric material (such as glass)
     */
    class Dielectric : public Material {
      public:

        /*!
         * Constructor taking index of refraction.
         */
        Dielectric(double ir) : ir_(ir) {}

        /*!
         * Destructor.
         */
        virtual ~Dielectric() {}

        /*!
         * Inherited from Material.
         */
        virtual bool scatter(const Ray& r_in, const hit_record& rec, Vector3d&
            attenuation, Ray& scattered) const override;

      public:
        double ir_; //!< Index of refraction
    };

    // Public Functions
    Vector3d reflect(const Vector3d& v, const Vector3d& n);

    Vector3d refract(const Vector3d& uv, const Vector3d& n, double etai_over_etat);

    double reflectance(double cosine, double ref_idx);

  } // namespace ray
} // namespace cannon


#endif /* ifndef CANNON_RAY_MATERIAL_H */
