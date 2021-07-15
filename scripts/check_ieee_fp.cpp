#include <cmath>

#include <cannon/log/registry.hpp>

using namespace cannon::log;

int main() {
  if (std::numeric_limits<double>::is_iec559) 
    log_info("Compiled with IEEE Floating Point support");
  else
    log_info("No IEEE Floating Point support");
}
