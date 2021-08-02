#include <cannon/math/interp.hpp>

#include <cmath>
#include <algorithm>

using namespace cannon::math;

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
