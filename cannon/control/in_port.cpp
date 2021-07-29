#include <cannon/control/in_port.hpp>

#include <cannon/control/signal.hpp>

using namespace cannon::control;

double InPort::value() const {
  return signal_->sample();
}
