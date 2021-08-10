#include <cannon/control/blocks/math.hpp>

#include <cannon/control/in_port.hpp>
#include <cannon/control/out_port.hpp>

using namespace cannon::control;

blocks::Add::Add() : in1_(new InPort), in2_(new InPort), out_(new OutPort) {}

blocks::Add::Add(SignalPtr in1, SignalPtr in2)
    : in1_(new InPort(in1)), in2_(new InPort(in2)), out_(new OutPort) {}

std::vector<InPortPtr> blocks::Add::in_ports() const {
  return {in1_, in2_};
}

std::vector<OutPortPtr> blocks::Add::out_ports() const {
  return {out_};
}

void blocks::Add::update(double dt, double t) {
  if (!is_valid())
    throw std::runtime_error("'Add' block not valid");

  out_->set_value(in1_->value() + in2_->value());
}

bool blocks::Add::is_valid() {
  return in1_->is_valid() && in2_->is_valid();
}

const SignalPtr blocks::Add::out_signal() {
  return out_->signal();
}

blocks::Gain::Gain() : multiplier_(1.0), in_(new InPort), out_(new OutPort) {}

blocks::Gain::Gain(double k)
    : multiplier_(k), in_(new InPort), out_(new OutPort) {}

std::vector<InPortPtr> blocks::Gain::in_ports() const {
  return { in_ };
}

std::vector<OutPortPtr> blocks::Gain::out_ports() const {
  return { out_};
}

void blocks::Gain::update(double dt, double t) {
  if (!is_valid())
    throw std::runtime_error("'Gain' block not valid");

  out_->set_value(in_->value() * multiplier_);
}

bool blocks::Gain::is_valid() {
  return in_->is_valid();
}
