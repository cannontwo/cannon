#include <cannon/math/interp.hpp>

#include <cmath>
#include <algorithm>
#include <cassert>

#include <cannon/log/registry.hpp>

using namespace cannon::log;
using namespace cannon::math;

// Spline methods

CubicSpline::CubicSpline(const std::vector<double> &xs,
                         const std::vector<double> &ys, BoundaryType bd,
                         double bd_first, double bd_last)
    : xs_(xs), ys_(ys), bd_(bd), bd_first_(bd_first), bd_last_(bd_last) {
  assert(xs.size() == ys.size());

  for (unsigned int i = 0; i < xs_.size() - 1; ++i) {
    assert(xs_[i] < xs_[i+1]);
  }
  
  a_coeffs_.resize(xs.size() - 1, 0.0);
  b_coeffs_.resize(xs.size() - 1, 0.0);
  c_coeffs_.resize(xs.size() - 1, 0.0);
  d_coeffs_.resize(xs.size() - 1, 0.0);

  compute_coeffs_();
}

CubicSpline::CubicSpline(const std::vector<Vector2d> &pts, BoundaryType bd,
                         double bd_first, double bd_last)
    : bd_(bd), bd_first_(bd_first), bd_last_(bd_last) {

  xs_.resize(pts.size());
  ys_.resize(pts.size());

  for (unsigned int i = 0; i < pts.size(); ++i) {
    xs_[i] = pts[i][0];
    ys_[i] = pts[i][1];
  }
  
  a_coeffs_.resize(pts.size() - 1, 0.0);
  b_coeffs_.resize(pts.size() - 1, 0.0);
  c_coeffs_.resize(pts.size() - 1, 0.0);
  d_coeffs_.resize(pts.size() - 1, 0.0);

  compute_coeffs_();
}

inline unsigned int CubicSpline::num_polys() const {
  return xs_.size() - 1;
}

double CubicSpline::operator()(double x) const {
  // Simple method, taken from
  // https://github.com/ttk592/spline/blob/master/src/spline.h
  
  unsigned int idx = find_closest_x_(x);

  return x * x * x * a_coeffs_[idx] + 
         x * x * b_coeffs_[idx] +
         x * c_coeffs_[idx] + 
         d_coeffs_[idx];
}

double CubicSpline::deriv(double x, unsigned int order) const {
  double df = 0.0;
  unsigned int idx = find_closest_x_(x);

  switch (order) {
    case 0:
      df = (*this)(x);
      break;
    case 1:
      df = 3 * a_coeffs_[idx] * x * x + 2 * b_coeffs_[idx] * x + c_coeffs_[idx];
      break;
    case 2:
      df = 6 * a_coeffs_[idx] * x + 2 * b_coeffs_[idx];
      break;
    case 3:
      df = 6 * a_coeffs_[idx];
      break;
    default:
      break;
  }

  return df;
}

void CubicSpline::compute_coeffs_() {
  SparseMatrix<double> A = construct_A_mat_();
  VectorXd b = construct_b_vec_();

  A.makeCompressed();

  SparseLU<SparseMatrix<double>> solver;
  solver.analyzePattern(A);
  solver.factorize(A);
  auto x = solver.solve(b);

  assert(x.size() == 4 * a_coeffs_.size());

  for (unsigned int i = 0; 4*i < x.size(); ++i) {
    a_coeffs_[i] = x[4*i];
    b_coeffs_[i] = x[4*i+1];
    c_coeffs_[i] = x[4*i+2];
    d_coeffs_[i] = x[4*i+3];
  }
}

SparseMatrix<double> CubicSpline::construct_A_mat_() const {
  SparseMatrix<double> ret_mat(4*num_polys(), 4*num_polys());

  std::vector<Triplet<double>> coeffs;

  // Value constraints
  for (unsigned int i = 0; i < num_polys(); ++i) {
    // First value constraint
    coeffs.push_back(Triplet<double>(2*i, 4*i, std::pow(xs_[i], 3)));
    coeffs.push_back(Triplet<double>(2*i, 4*i + 1, xs_[i] * xs_[i]));
    coeffs.push_back(Triplet<double>(2*i, 4*i + 2, xs_[i]));
    coeffs.push_back(Triplet<double>(2*i, 4*i + 3, 1.0));

    // Second value constraint
    coeffs.push_back(Triplet<double>(2*i + 1, 4*i, std::pow(xs_[i+1], 3)));
    coeffs.push_back(Triplet<double>(2*i + 1, 4*i + 1, xs_[i+1] * xs_[i+1]));
    coeffs.push_back(Triplet<double>(2*i + 1, 4*i + 2, xs_[i+1]));
    coeffs.push_back(Triplet<double>(2*i + 1, 4*i + 3, 1.0));
  }

  unsigned int rows_so_far = 2*num_polys();

  // First derivative constraints
  for (unsigned int i = 0; i < num_polys() - 1; ++i) {
    // Positive portion
    coeffs.push_back(Triplet<double>(i + rows_so_far, 4*i, 3 * xs_[i+1] * xs_[i+1]));
    coeffs.push_back(Triplet<double>(i + rows_so_far, 4*i + 1, 2 * xs_[i+1]));
    coeffs.push_back(Triplet<double>(i + rows_so_far, 4*i + 2, 1));

    // Negative portion
    coeffs.push_back(Triplet<double>(i + rows_so_far, 4*(i+1), -3 * xs_[i+1] * xs_[i+1]));
    coeffs.push_back(Triplet<double>(i + rows_so_far, 4*(i+1) + 1, -2 * xs_[i+1]));
    coeffs.push_back(Triplet<double>(i + rows_so_far, 4*(i+1) + 2, -1));
  }

  rows_so_far += num_polys() - 1;

  // Second derivative constraints
  for (unsigned int i = 0; i < num_polys() - 1; ++i) {
    // Positive portion
    coeffs.push_back(Triplet<double>(i + rows_so_far, 4*i, 6 * xs_[i+1]));
    coeffs.push_back(Triplet<double>(i + rows_so_far, 4*i + 1, 2));

    // Negative portion
    coeffs.push_back(Triplet<double>(i + rows_so_far, 4*(i+1), -6 * xs_[i+1]));
    coeffs.push_back(Triplet<double>(i + rows_so_far, 4*(i+1) + 1, -2));
  }

  rows_so_far += num_polys() - 1;
  
  // Last two rows are used for boundary conditions.
  switch (bd_) {
    case BoundaryType::FirstDeriv:
      // First derivative
      coeffs.push_back(Triplet<double>(rows_so_far, 0, 3 * xs_[0] * xs_[0]));
      coeffs.push_back(Triplet<double>(rows_so_far, 1, 2 * xs_[0]));
      coeffs.push_back(Triplet<double>(rows_so_far, 2, 1));

      ++rows_so_far;

      // Last derivative
      coeffs.push_back(Triplet<double>(rows_so_far, 4*(num_polys()-1), 3 * xs_[num_polys()] * xs_[num_polys()]));
      coeffs.push_back(Triplet<double>(rows_so_far, 4*(num_polys()-1) + 1, 2 * xs_[num_polys()]));
      coeffs.push_back(Triplet<double>(rows_so_far, 4*(num_polys()-1) + 2, 1));

      ++rows_so_far;
      
      break;
    case BoundaryType::SecondDeriv:
      // First curvature
      coeffs.push_back(Triplet<double>(rows_so_far, 0, 3 * xs_[0]));
      coeffs.push_back(Triplet<double>(rows_so_far, 1, 2));

      ++rows_so_far;

      // Last curvature
      coeffs.push_back(Triplet<double>(rows_so_far, 4*(num_polys()-1), 3 * xs_[num_polys()]));
      coeffs.push_back(Triplet<double>(rows_so_far, 4*(num_polys()-1) + 1, 2));

      ++rows_so_far;
      break;
    case BoundaryType::NotAKnot:
      // First bound
      coeffs.push_back(Triplet<double>(rows_so_far, 0, 1));
      coeffs.push_back(Triplet<double>(rows_so_far, 4, -1));

      ++rows_so_far;

      // Last bound
      coeffs.push_back(Triplet<double>(rows_so_far, 4*(num_polys()-2), 1));
      coeffs.push_back(Triplet<double>(rows_so_far, 4*(num_polys()-1), -1));

      ++rows_so_far;
      break;
  }

  assert(rows_so_far == 4*num_polys());
  
  ret_mat.setFromTriplets(coeffs.begin(), coeffs.end());

  return ret_mat;
}

VectorXd CubicSpline::construct_b_vec_() const {
  VectorXd ret_vec = VectorXd::Zero(4*num_polys());
  ret_vec[0] = ys_[0];

  for (unsigned int i = 1; i < num_polys(); ++i) {
    ret_vec[2*i - 1] = ys_[i]; 
    ret_vec[2*i] = ys_[i]; 
  }

  ret_vec[2*num_polys() - 1] = ys_[num_polys()];

  switch (bd_) {
    case BoundaryType::FirstDeriv:
      // Last two rows are used for boundary conditions.
      ret_vec[4*num_polys() - 2] = bd_first_;
      ret_vec[4*num_polys() - 1] = bd_last_;
      break;
    case BoundaryType::SecondDeriv:
      // Last two rows are used for boundary conditions.
      ret_vec[4*num_polys() - 2] = bd_first_;
      ret_vec[4*num_polys() - 1] = bd_last_;
      break;
    case BoundaryType::NotAKnot:
      // Nothing necessary in b vector for not-a-knot
      break;
  }

  return ret_vec;
}

unsigned int CubicSpline::find_closest_x_(double x) const {
  auto it = std::upper_bound(xs_.begin(), xs_.end(), x); 
  
  if (it == xs_.end())
    return num_polys() - 1;
  else {
    unsigned int idx = std::max(int(it - xs_.begin()) - 1, 0);
    return idx;
  }
}

MultiSpline::MultiSpline(const std::vector<double> &times,
                         const std::vector<VectorXd> &coords,
                         CubicSpline::BoundaryType bd, double bd_first,
                         double bd_last) {

  assert(times.size() == coords.size());

  dim_ = coords[0].size();
  for (auto& c : coords) {
    assert(c.size() == dim_);
  }

  for (unsigned int i = 0; i < dim_; ++i) {
    std::vector<double> coord_vec;
    for (auto& c : coords) {
      coord_vec.push_back(c[i]);
    }

    coord_splines_.push_back(
        std::make_shared<CubicSpline>(times, coord_vec, bd, bd_first, bd_last));
  }
}

VectorXd MultiSpline::operator()(double t) const {
  VectorXd ret_vec = VectorXd::Zero(dim_);

  for (unsigned int i = 0; i < dim_; ++i) {
    ret_vec[i] = (*coord_splines_[i])(t);
  }

  return ret_vec;
}

VectorXd MultiSpline::deriv(double t, unsigned int order) const {
  VectorXd ret_vec = VectorXd::Zero(dim_);

  for (unsigned int i = 0; i < dim_; ++i) {
    ret_vec[i] = coord_splines_[i]->deriv(t, order);
  }

  return ret_vec;
}

// Free Functions

std::vector<double> cannon::math::cheb_points(unsigned int num, double low, double high) {
  std::vector<double> ret_vec;

  double mid = (low + high) / 2.0;
  double diff = (high - low) / 2.0;
  double mult = M_PI / (2*num);

  for (unsigned int i = 0; i < num; ++i) {
    ret_vec.push_back(mid + diff * std::cos(mult * (2*i + 1)));
  }

  std::reverse(ret_vec.begin(), ret_vec.end());

  return ret_vec;
}

std::function<double(double)>
cannon::math::lagrange_interp(std::function<double(double)> f,
                              const std::vector<double>& pts) {
  std::vector<double> y_vec;
  for (auto& pt : pts)
    y_vec.push_back(f(pt));

  // Compute l_j(x)
  std::vector<double> denoms;
  for (unsigned int i = 0; i < pts.size(); ++i) {
    double prod = 1;
    for (unsigned int j = 0; j < pts.size(); ++j) {
      if (i != j) {
        prod *= pts[i] - pts[j];
      }
    }
    denoms.push_back(prod);
  }
  

  return [y_vec, denoms, pts](double x) {
    double sum = 0.0;

    for (unsigned int i = 0; i < pts.size(); ++i) {
      double prod = 1.0;
      for (unsigned int j = 0; j < pts.size(); ++j) {
        if (i != j) {
          prod *= x - pts[j];
        }
      }

      sum += (prod / denoms[i]) * y_vec[i];
    }

    return sum;
  };
}

VectorXd cannon::math::lerp(const Ref<const VectorXd> &a,
                            const Ref<const VectorXd> &b, double t) {
  assert(0.0 <= t && t <= 1.0);

  return (1.0 - t) * a + t * b;
}
