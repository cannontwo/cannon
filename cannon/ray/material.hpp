#pragma once
#ifndef CANNON_RAY_MATERIAL_H
#define CANNON_RAY_MATERIAL_H 

/*!
 * \file cannon/ray/material.hpp
 * \brief File containing class definitions for Material and derived materials.
 */

#include <Eigen/Dense>

#include <cannon/math/random_double.hpp>
#include <cannon/utils/class_forward.hpp>

using namespace Eigen;

using namespace cannon::math;

namespace cannon {
  namespace ray {

    CANNON_CLASS_FORWARD(Ray);
    CANNON_CLASS_FORWARD(Texture);
    CANNON_CLASS_FORWARD(hit_record);

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

        /*!
         * Method that returns the emitted color for this material at a
         * specified point.
         *
         * \param u Horizontal surface coordinate.
         * \param v Vertical surface coordinate.
         * \param p Hit point from which light is being emitted.
         *
         * \returns Emitted color.
         */
        virtual Vector3d emitted(double u, double v, const Vector3d& p) const {
          return Vector3d::Zero();
        }
    };

    /*!
     * \brief Class representing a debugging material displaying normal direction.
     */
    class NormalDebug : public Material {
      public:

        /*!
         * Destructor.
         */
        virtual ~NormalDebug() {}

        /*!
         * Inherited from Material.
         */
        virtual bool scatter(const Ray& r_in, const hit_record& rec, Vector3d&
            attenuation, Ray& scattered) const override;

    };

    /*!
     * \brief Class representing a Lambertian diffuse material.
     */
    class Lambertian : public Material {
      public:

        /*!
         * Constructor taking albedo color.
         */
        Lambertian(const Vector3d& a);

        /*!
         * Constructor taking albedo texture.
         */
        Lambertian(TexturePtr a) : albedo_(a) {}

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
        TexturePtr albedo_; //!< Albedo color for this material.

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

    class DiffuseLight : public Material {
      public:

        /*!
         * Constructor taking a texture for this material.
         */
        DiffuseLight(TexturePtr a) : emit_(a) {}

        /*!
         * Constructor taking a color for this material.
         */
        DiffuseLight(const Vector3d& c);

        /*!
         * Destructor.
         */
        virtual ~DiffuseLight() {}

        /*!
         * Inherited from Material.
         */
        virtual bool scatter(const Ray& r_in, const hit_record& rec, Vector3d&
            attenuation, Ray& scattered) const override {
          return false;
        }

        /*!
         * Inherited from Material.
         */
        virtual Vector3d emitted(double u, double v, const Vector3d& p) const override;

      public:
        TexturePtr emit_; //!< Emissive texture
    };

    class Isotropic : public Material {
      public:

        /*!
         * Constructor taking a color for this material.
         */
        Isotropic(const Vector3d& c);

        /*!
         * Constructor taking a texture for this material.
         */
        Isotropic(TexturePtr a) : albedo_(a) {}

        /*!
         * Destructor.
         */
        virtual ~Isotropic() {}

        /*!
         * Inherited from Material.
         */
        virtual bool scatter(const Ray& r_in, const hit_record& rec, Vector3d&
            attenuation, Ray& scattered) const override;


      public:
        TexturePtr albedo_; //!< Albedo for this material

    };

    // Public Functions
    
    /*!
     * Function that computes reflected direction for an incoming ray.
     *
     * \param v The ray direction to reflect.
     * \param n The normal to reflect around.
     *
     * \returns The reflected ray direction.
     */
    Vector3d reflect(const Vector3d& v, const Vector3d& n);

    /*!
     * Function that computes the direction for refraction of a direction.
     *
     * \param uv Direction of ray to refract.
     * \param n Normal direction of the refracting surface.
     * \param etai_over_etat Index of refraction ratio.
     *
     * \returns Refracted direction.
     */
    Vector3d refract(const Vector3d& uv, const Vector3d& n, double etai_over_etat);

    /*!
     * Computes approximate reflectance for a dielectric.
     */
    double reflectance(double cosine, double ref_idx);

  } // namespace ray
} // namespace cannon


#endif /* ifndef CANNON_RAY_MATERIAL_H */
