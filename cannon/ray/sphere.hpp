#pragma once
#ifndef CANNON_RAY_SPHERE_H
#define CANNON_RAY_SPHERE_H 

/*!
 * \file cannon/ray/sphere.hpp
 * \brief File containing Sphere class definition.
 */

#include <cannon/ray/hittable.hpp>
#include <cannon/utils/class_forward.hpp>

namespace cannon {
  namespace ray {

    CANNON_CLASS_FORWARD(Ray);

    /*!
     * \brief Class representing a raytraceable sphere.
     */
    class Sphere : public Hittable {
      public:
        /*!
         * Default constructor
         */
        Sphere() {}

        /*!
         * Constructor taking a sphere center, radius, and material.
         */
        Sphere(const Vector3d& center, double r, std::shared_ptr<Material>
            mat_ptr) : center_(center), radius_(r), mat_ptr_(mat_ptr) {}

        /*!
         * Destructor.
         */
        virtual ~Sphere() {}

        /*!
         * Inherited from Hittable.
         */
        virtual bool object_space_hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;

        /*!
         * Inherited from Hittable.
         */
        virtual bool object_space_bounding_box(double time_0, double time_1, Aabb& output_box) const override;

      public:
        Vector3d center_; //!< Sphere center
        double radius_; //!< Sphere radius
        std::shared_ptr<Material> mat_ptr_; //!< Material for this sphere

    };

    // Free Functions
    
    /*!
     * Compute surface texture U,V coordinates for a sphere.
     *
     * \param p Point on the unit sphere centered at the origin.
     * \param u Returned value in [0, 1] of angle around the Y axis from X=-1
     * \param v Returned value in [0, 1] of angle from Y=-1 to Y=+1
     */
    void get_sphere_uv(const Vector3d& p, double& u, double& v);

  } // namespace ray
} // namespace cannon

#endif /* ifndef CANNON_RAY_SPHERE_H */
