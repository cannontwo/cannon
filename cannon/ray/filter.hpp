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

    /*!
     * \brief Class representing a triangle filter, which is not much better
     * than a box filter.
     */
    class TriangleFilter : public Filter {
      public:

        /*!
         * \brief Constructor taking the radius of this filter.
         */
        TriangleFilter(const Vector2d& radius) : Filter(radius) {}

        /*!
         * \brief Destructor.
         */
        virtual ~TriangleFilter() {}

        /*!
         * \brief Inherited from Filter.
         */
        virtual double evaluate(const Vector2d& p) const override {
          return std::max(0.0, radius_.x() - std::abs(p.x())) *
                 std::max(0.0, radius_.y() - std::abs(p.y()));
        }
    };

    /*!
     * \brief Class representing a Gaussian filter. Reasonable for image
     * reconstruction.
     */
    class GaussianFilter : public Filter {
      public:

        /*!
         * \brief Constructor taking the radius of this filter and rate of
         * Gaussian falloff.
         */
        GaussianFilter(const Vector2d &radius, double alpha)
            : Filter(radius), alpha_(alpha * radius.x()),
              exp_x_(std::exp(-alpha * radius.x() * radius.x())),
              exp_y_(std::exp(-alpha * radius.y() * radius.y())) {}

        /*!
         * \brief Destructor.
         */
        virtual ~GaussianFilter() {}

        /*!
         * \brief Inherited from Filter.
         */
        virtual double evaluate(const Vector2d& p) const override {
          return gaussian_(p.x(), exp_x_) * gaussian_(p.y(), exp_y_);
        }

      private:

        /*!
         * \brief 1D Gaussian function, used to compute 2D Gaussian filter
         * value since this filter is separable.
         *
         * \param d Distance from center of filter
         * \param exp_v Constant term
         */
        double gaussian_(double d, double exp_v) const {
          return std::max(0.0, std::exp(-alpha_ * d * d) - exp_v);
        }


        const double alpha_; //!< Gaussian falloff rate
        const double exp_x_; //!< Cached constant term in x
        const double exp_y_; //!< Cached constant term in y
    };

    /*!
     * \brief Class representing a Mitchell-Netravali filter. Reasonable for image
     * reconstruction.
     */
    class MitchellFilter : public Filter {
      public:

        /*!
         * \brief Constructor taking the radius of this filter and
         * Mitchell-Netravali parameters B and C.
         */
        MitchellFilter(const Vector2d &radius, double b, double c)
            : Filter(radius), b_(b), c_(c) {}

        /*!
         * \brief Destructor.
         */
        virtual ~MitchellFilter() {}

        /*!
         * \brief Inherited from Filter.
         */
        virtual double evaluate(const Vector2d& p) const override {
          return mitchell_1d_(p.x() * inv_radius_.x()) * mitchell_1d_(p.y() * inv_radius_.y());
        }

      private:

        /*!
         * \brief 1D Mitchell function, used to compute 2D Mitchell filter
         * value since this filter is separable.
         *
         * \param x Scaled distance from center of filter
         */
        double mitchell_1d_(double x) const {
          x = std::abs(2 * x);

          if (x > 1)
            return ((-b_ - 6*c_) * x*x*x + (6*b_ + 30*c_) * x*x + 
                    (-12*b_ - 48*c_) * x + (8*b_ + 24*c_)) * (1.0 / 6.0);
          else
            return ((12 - 9*b_ - 6*c_) * x*x*x +
                    (-18 + 12*b_ + 6*c_) * x*x +
                    (6 - 2*b_)) * (1.0 / 6.0);
        }

        double b_;
        double c_;

    };


  } // namespace ray
} // namespace cannon

#endif /* ifndef CANNON_RAY_FILTER_H */
