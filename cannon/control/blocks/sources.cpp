#include <cannon/control/blocks/sources.hpp>

#include <cannon/control/out_port.hpp>

using namespace cannon::control;

blocks::Constant::Constant() : value_(0.0), out_(new OutPort) {
  out_->set_value(value_);
}

blocks::Constant::Constant(double value) : value_(value), out_(new OutPort) {
  out_->set_value(value_);
}

std::vector<InPortPtr> blocks::Constant::in_ports() const {
  return {};
}

std::vector<OutPortPtr> blocks::Constant::out_ports() const {
  return {out_};
}

void blocks::Constant::update() {
  out_->set_value(value_);
}

bool blocks::Constant::is_valid() {
  return true;
}

const SignalPtr blocks::Constant::out_signal() {
  return out_->signal();
}
