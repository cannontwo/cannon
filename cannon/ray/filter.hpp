#pragma once
#ifndef CANNON_RAY_FILTER_H
#define CANNON_RAY_FILTER_H

/*!
 * \file cannon/ray/filter.hpp
 * File containing class definitions for Film reconstruction filters.
 */

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace ray {

    /*!
     * \brief Abstract base class representing a reconstruction filter for
     * integrating image samples.
     */
    class Filter {
      public:

        /*!
         * \brief Constructor taking a radius for this filter.
         */
        Filter(const Vector2d& radius) : radius_(radius), inv_radius_(1.0 /
            radius.x(), 1.0 / radius.y()) {}

        /*!
         * \brief Get the value of this filter at the input point. The filter
         * is assumed to be centered at the origin, so the input point should
         * be with respect to the actual filter position.
         *
         * \param p The point to evaluate the filter for.
         *
         * \returns The value of this filter at the point.
         */
        virtual double evaluate(const Vector2d& p) const = 0;

        /*!
         * \brief Destructor.
         */
        virtual ~Filter() {}

      public:
        const Vector2d radius_; //!< Radius of this filter
        const Vector2d inv_radius_; //!< 1 / radius, speeds up math

    };

    /*!
     * \brief Class representing the most basic image reconstruction filter,
     * the Box Filter.
     */
    class BoxFilter : public Filter {
      public:

        /*!
         * \brief Constructor taking the radius of this filter.
         */
        BoxFilter(const Vector2d& radius) : Filter(radius) {}

        /*!
         * \brief Destructor.
         */
        virtual ~BoxFilter() {}

        /*!
         * \brief Inherited from Filter.
         */
        virtual double evaluate(const Vector2d& p) const override {
          return 1.0;
        }

    };

  } // namespace ray
} // namespace cannon

#endif /* ifndef CANNON_RAY_FILTER_H */
