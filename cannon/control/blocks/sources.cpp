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

void blocks::Constant::update(double /*dt*/, double /*t*/) {
  out_->set_value(value_);
}

bool blocks::Constant::is_valid() {
  return true;
}

const SignalPtr blocks::Constant::out_signal() {
  return out_->signal();
}

blocks::Step::Step() : Step(0.0, 1.0, 1.0) {}

blocks::Step::Step(double initial_value, double final_value, double step_time)
    : initial_value_(initial_value), final_value_(final_value),
      step_time_(step_time), out_(new OutPort) {
  out_->set_value(initial_value_);
}

std::vector<InPortPtr> blocks::Step::in_ports() const {
  return {};
}

std::vector<OutPortPtr> blocks::Step::out_ports() const {
  return {out_};
}

void blocks::Step::update(double /*dt*/, double t) {
  out_->set_value(t < step_time_ ? initial_value_ : final_value_);
}

bool blocks::Step::is_valid() {
  return true;
}

const SignalPtr blocks::Step::out_signal() {
  return out_->signal();
}
