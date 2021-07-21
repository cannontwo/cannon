#include <catch2/catch.hpp>

#include <cannon/log/registry.hpp>
#include <cannon/math/fft.hpp>
#include <cannon/math/nearly_equal.hpp>

using namespace cannon::math;
using namespace cannon::log;

TEST_CASE("FFT", "[math]") {
  std::vector<std::complex<double>> data;
  data.push_back(0.0);
  data.push_back(1.0);
  data.push_back(2.0);
  data.push_back(3.0);
  data.push_back(4.0);
  data.push_back(5.0);
  data.push_back(6.0);
  data.push_back(7.0);

  bit_reverse(data);

  log_info("Bit-reversed array is");
  for (auto& x : data)
    log_info("\t", x);

  REQUIRE(data[0] == std::complex<double>{0});
  REQUIRE(data[1] == std::complex<double>{4});
  REQUIRE(data[2] == std::complex<double>{2});
  REQUIRE(data[3] == std::complex<double>{6});
  REQUIRE(data[4] == std::complex<double>{1});
  REQUIRE(data[5] == std::complex<double>{5});
  REQUIRE(data[6] == std::complex<double>{3});
  REQUIRE(data[7] == std::complex<double>{7});

  bit_reverse(data);
  log_info("Un-reversed array is");
  for (auto& x : data)
    log_info("\t", x);

  auto freq_data = fft(data);
  log_info("FFT(data) is");
  for (auto& x : freq_data)
    log_info("\t", x);

  auto inv_freq_data = fft(freq_data, true);
  log_info("IFFT(FFT(data)) is");
  for (auto& x : inv_freq_data)
    log_info("\t", x);

  for (unsigned int i = 0; i < data.size(); ++i) {
    log_info("data[i] =", data[i], "inv_freq_data[i] =", inv_freq_data[i]);
    REQUIRE(std::abs(data[i].real() - inv_freq_data[i].real()) < 1e-12);
    REQUIRE(std::abs(data[i].imag() - inv_freq_data[i].imag()) < 1e-12);
  }

}
