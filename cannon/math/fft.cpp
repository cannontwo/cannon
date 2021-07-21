#include <cannon/math/fft.hpp>

#include <cannon/log/registry.hpp>

using namespace cannon::math;
using namespace cannon::log;

void cannon::math::bit_reverse(std::vector<std::complex<double>>& data) {
  unsigned int j = 0;

  for (unsigned int i = 0; i < data.size(); ++i) {
    if (j > i)
      std::swap(data[j], data[i]);

    unsigned int m = data.size() >> 1;

    while (m >= 1 && j >= m) {
      j -= m;
      m >>= 1;
    }

    j += m;
  }
}

// Adapted from the algorithm in Numerical Recipes by Press et al., pg. 612
std::vector<std::complex<double>>
cannon::math::fft(std::vector<std::complex<double>> data, bool inverse) {

  if (data.size() < 2 || data.size() & (data.size() - 1))
    throw std::runtime_error("Data passed to fft() must have power of two length.");

  // Rearrange data in bit-reversed order
  bit_reverse(data);

  // Danielson-Lanczos portion of FFT routine
  unsigned int mmax = 1;
  while (data.size() > mmax) {
    unsigned int i_step = mmax << 1;

    // For trigonometric recurrence
    double theta = (inverse ? -1.0 : 1.0) * (M_PI / mmax);
    double wtemp = std::sin(0.5 * theta);
    double wpr = -2.0 * wtemp * wtemp; // -2 sin^2(\pi / mmax)
    double wpi = std::sin(theta);           // sin(2*\pi / mmax)
    double wr = 1.0;
    double wi = 0.0;

    for (unsigned int m = 0; m < mmax; ++m) {
      for (unsigned int i = m; i < data.size(); i += i_step) {
        unsigned int j = i + mmax;

        // Danielson-Lanczos formula
        double tempr = wr * data[j].real() - wi*data[j].imag();
        double tempi = wr * data[j].imag() + wi*data[j].real();
        data[j].real(data[i].real() - tempr);
        data[j].imag(data[i].imag() - tempi);
        
        data[i] += std::complex<double>(tempr, tempi);
      }

      // Trigonometric recurrence (see
      // https://en.wikipedia.org/wiki/Trigonometric_tables)
      wtemp = wr;
      wr += wtemp*wpr - wi*wpi;
      wi += wi*wpr + wtemp*wpi;
    }

    mmax = i_step;
  }

  // Extra factor of 1/N when doing the inverse transform
  if (inverse) {
    for (auto& x : data) 
      x /= data.size();
  }

  return data;
}
