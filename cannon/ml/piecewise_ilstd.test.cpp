#include <catch2/catch.hpp>

#include <cannon/ml/piecewise_ilstd.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::ml;
using namespace cannon::log;

TEST_CASE("PiecewiseILSTD", "[ml]") {
  PiecewiseILSTDFilter f(2, 10, 0.95);

  VectorXd in1(2);
  in1 << 1.0,
         0.0;
  VectorXd in2(2);
  in2 << 0.0,
         1.0;

  for (unsigned int i = 0; i < 1000; ++i) {
    f.process_datum(in1, in2, 0, 0, 1.0);
  }

  log_info(f.get_mat(0));
  log_info(f.predict(in1, 0));
}
