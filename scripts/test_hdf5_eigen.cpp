#include <Eigen/Dense>
#include <thirdparty/HighFive/include/highfive/H5Easy.hpp>

#include <cannon/log/registry.hpp>

using namespace Eigen;

using namespace cannon::log;

int main() {
  H5Easy::File file("test.h5", H5Easy::File::Overwrite);

  Matrix2d m;
  m << 1.0, 2.0,
       3.0, 4.0;

  H5Easy::dump(file, "/tmp/m", m);

  Matrix2d m2 = H5Easy::load<Matrix2d>(file, "/tmp/m");

  log_info("m is", m);
  log_info("m2 is", m2);
}
