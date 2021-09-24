#ifndef CANNON_MATH_INTERP_H
#define CANNON_MATH_INTERP_H 

/*!
 * \file cannon/math/interp.hpp
 * \brief File containing utilities for doing function interpolation.
 */

#include <vector>
#include <functional>
#include <memory>

#include <Eigen/Dense>
#include <Eigen/Sparse>

using namespace Eigen;

namespace cannon {
  namespace math {

    /*!
     * \brief Class representing the cubic spline interpolation of a set of
     * points. Following https://timodenk.com/blog/cubic-spline-interpolation/.
     */
    class CubicSpline {
      public:

        enum class BoundaryType {
          FirstDeriv,
          SecondDeriv,
          NotAKnot
        };

        CubicSpline() = delete;

        /*!
         * \brief Constructor taking data to interpolate, boundary type for the
         * spline, and values of boundary conditions. Data should be sorted in
         * increasing order of xs.
         */
        CubicSpline(const std::vector<double>& xs, const std::vector<double>& ys,
                    BoundaryType bd = BoundaryType::SecondDeriv,
                    double bd_first = 0.0, double bd_last = 0.0);
        /*!
         * \brief Constructor taking data to interpolate, boundary type for the
         * spline, and values of boundary conditions. Data should be sorted in
         * increasing order of xs.
         */
        CubicSpline(const std::vector<Vector2d>& pts,
                    BoundaryType bd = BoundaryType::SecondDeriv,
                    double bd_first = 0.0, double bd_last = 0.0);

        /*!
         * \brief Functional operator. 
         */
        double operator()(double x) const;

        /*!
         * \brief Get the derivative of this spline of input order at the input
         * point.
         *
         * \param x The point to evaluate curve derivative at.
         * \param order Derivative order to evaluate.
         *
         * \returns The derivative.
         */
        double deriv(double x, unsigned int order = 1) const;

        /*!
         * \brief Get the number of polynomials defining this spline.
         *
         * \returns Number of polynomials.
         */
        inline unsigned int num_polys() const;

      private:
        /*!
         * \brief Compute coefficients for spline from data points and boundary
         * conditions.
         */
        void compute_coeffs_();

        /*!
         * \brief Compute A matrix for system of equations defining cubic
         * spline.
         *
         * \returns A matrix which will be solved to compute spline
         * coefficients.
         */
        SparseMatrix<double> construct_A_mat_() const;

        /*!
         * \brief Compute b vector for system of equations defining cubic
         * spline.
         *
         * \returns b vector which will be used to solve system of equations
         * for spline coefficients.
         */
        VectorXd construct_b_vec_() const;

        /*!
         * \brief Find the closest idx such that xs_[idx] <= x
         *
         * \param x Input value to locate
         *
         * \returns Index of closest x datum
         */
        unsigned int find_closest_x_(double x) const;

        std::vector<double> xs_; //!< X-values of data
        std::vector<double> ys_; //!< Y-values of data

        const BoundaryType bd_; //!< Type of boundary condition for this spline
        const double bd_first_; //!< First boundary condition value
        const double bd_last_; //!< Last boundary condition value

        std::vector<double> a_coeffs_; //!< Coefficients of x^3 for each polynomial
        std::vector<double> b_coeffs_; //!< Coefficients of x^2 for each polynomial
        std::vector<double> c_coeffs_; //!< Coefficients of x for each polynomial
        std::vector<double> d_coeffs_; //!< Constant coefficients for each polynomial

    };

    /*!
     * \brief Class representing a multivariate spline, defined by its
     * component coordinate splines.
     */
    class MultiSpline {
      public:

        MultiSpline() = delete;

        /*!
         * \brief Constructor taking trajectory times and coordinate points, as
         * well as boundary conditions that will be set uniformly for each
         * spline.
         */
        MultiSpline(
            const std::vector<double> &times, const std::vector<VectorXd> &coords,
            CubicSpline::BoundaryType bd = CubicSpline::BoundaryType::SecondDeriv,
            double bd_first = 0.0, double bd_last = 0.0);

        /*!
         * \brief Functor operator taking a time and returning the coordinates
         * of the multispline at that time.
         *
         * \param t Time to evaluate spline at.
         *
         * \returns Value of spline at that time.
         */
        VectorXd operator()(double t) const;

        /*!
         * \brief Compute derivative with respect to time parameterization of
         * this cubic spline.
         *
         * \param t The time to get derivative at.
         * \param order The order of derivative to compute.
         *
         * \returns The derivative.
         */
        VectorXd deriv(double t, unsigned int order = 1) const;

        /*!
         * \brief Compute a normal vector for this spline at the input time.
         *
         * \param t The time to compute a normal for.
         *
         * \returns The computed normal. 
         */
        VectorXd normal(double t) const;


      private:
        std::vector<std::shared_ptr<CubicSpline>> coord_splines_; //!< Coordinate spline functions
        unsigned int dim_; //!< Dimension of spline
    };

    /*!
     * \brief Compute the Chebyshev points between the input bounds. See
     * https://en.wikipedia.org/wiki/Chebyshev_nodes 
     *
     * \param num Number of points to generate
     * \param low Lower limit for points
     * \param high Upper limit for points
     *
     * \returns A vector containing the Chebyshev points.
     */
    std::vector<double> cheb_points(unsigned int num, double low=-1.0, double high=1.0);

    /*!
     * \brief Compute the Lagrange interpolant for the input function on the
     * input points.
     *
     * \param f The function to interpolate.
     * \param pts The points to interpolate on.
     *
     * \returns The Lagrange interpolating polynomial on the input points.
     */
    std::function<double(double)>
    lagrange_interp(std::function<double(double)> f, const std::vector<double>& pts);

    /*!
     * \brief Interpolate between the two input vectors.
     *
     * \param a First vector
     * \param b Second vector
     * \param t Interpolation amount. Should be between 0 and 1
     *
     * \returns Interpolated vector.
     */
    VectorXd lerp(const Ref<const VectorXd> &a, const Ref<const VectorXd> &b,
                  double t);

    /*!
     * \brief Compute the projection of the first vector onto the second.
     *
     * \param a The vector to be projected
     * \param b The vector that projection is computed with respect to.
     *
     * \returns The projected vector.
     */
    VectorXd projection(const Ref<const VectorXd> &a,
                        const Ref<const VectorXd> &b);

    /*!
     * \brief Compute the complementary projection of the first vector with
     * respect to the second.
     *
     * \param a The vector to be projected
     * \param b The vector that projection is computed with respect to.
     *
     * \returns The complementary projection.
     */
    VectorXd complementary_projection(const Ref<const VectorXd> &a,
                                      const Ref<const VectorXd> &b);
  }
} 

#endif /* ifndef CANNON_MATH_INTERP_H */
