#include <cannon/control/out_port.hpp>

#include <cannon/control/signal.hpp>

using namespace cannon::control;

OutPort::OutPort() : signal_(new Signal) {}

double OutPort::value() const {
  return signal_->sample();
}

void OutPort::set_value(double val) {
  signal_->set_value(val);
}
