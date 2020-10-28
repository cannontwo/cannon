#include <cannon/ml/piecewise_lstd.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::ml;
using namespace cannon::log;

int main() {
  PiecewiseLSTDFilter f(2, 10, 0.95);

  VectorXd in1(2);
  in1 << 1.0,
         0.0;
  VectorXd in2(2);
  in2 << 0.0,
         1.0;

  f.process_datum(in1, in2, 0, 0, 1.0);
  log_info(f.get_mat(0));
  log_info(f.predict(in1, 0));
}
