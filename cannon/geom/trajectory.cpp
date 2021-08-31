#include <cannon/geom/trajectory.hpp>

#include <stdexcept>

#include <cannon/math/interp.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::geom;
using namespace cannon::math;
using namespace cannon::log;

Trajectory::Trajectory() {}

Trajectory::Trajectory(const std::vector<VectorXd> &states,
                       const std::vector<double> times)
    : states_(states), times_(times) {
  if (states.size() != times.size())
    throw std::runtime_error("States vector and times vector passed to "
                             "Trajectory have different sizes");

  for (auto& t : times) {
    if (t < 0.0)
      throw std::runtime_error("Trajectory times should be >= 0");
  }
}

VectorXd Trajectory::operator()(double t) const {

  if (times_.empty())
    throw std::runtime_error("No states in trajectory");

  // We pause at the end and before the beginning
  if (t <= times_[0])
    return states_[0];
  else if (t >= times_.back()) 
    return states_.back();
  else {
    unsigned int idx = find_closest_t_(t);

    assert(idx != times_.size() - 1);

    double prev_time = times_[idx];
    double next_time = times_[idx+1];
    assert(t >= prev_time && t <= next_time);

    double diff = next_time - prev_time;
    double prog = t - prev_time;
    double x = prog / diff;

    return lerp(states_[idx], states_[idx+1], x);
  }
}

void Trajectory::push_back(const Ref<const VectorXd>& state, double time) {
  if (!times_.empty()) {
    if (time < times_.back())
      throw std::runtime_error("Time passed to trajectory is decreasing");

    if (time < 0.0)
      throw std::runtime_error("Times should be >= 0");
  }

  states_.push_back(state);
  times_.push_back(time);
}

double Trajectory::length() const {
  if (times_.empty())
    return 0.0;
  else
    return times_.back();
}

size_t Trajectory::size() const {
  return states_.size();
}

MultiSpline Trajectory::interp() const {
  // Zero velocity at beginning and end
  return MultiSpline(times_, states_, CubicSpline::BoundaryType::FirstDeriv,
                     0.0, 0.0);
}

unsigned int Trajectory::find_closest_t_(double t) const {
  auto it = std::upper_bound(times_.begin(), times_.end(), t); 
  
  if (it == times_.end())
    return times_.size() - 1;
  else {
    unsigned int idx = std::max(int(it - times_.begin()) - 1, 0);
    return idx;
  }
}

ControlledTrajectory::ControlledTrajectory() {}

ControlledTrajectory::ControlledTrajectory(
    const std::vector<VectorXd> &states, const std::vector<VectorXd> &controls,
    const std::vector<double> times) : states_(states), controls_(controls), times_(times) {

  if (states.size() != times.size())
    throw std::runtime_error("States vector and times vector passed to "
                             "Trajectory have different sizes");

  if (states.size() != controls.size())
    throw std::runtime_error("States vector and controls vector passed to "
                             "Trajectory have different sizes");

  for (auto& t : times) {
    if (t < 0.0)
      throw std::runtime_error("Trajectory times should be >= 0");
  }
}

std::pair<VectorXd, VectorXd> ControlledTrajectory::operator()(double t) const {
  if (times_.empty())
    throw std::runtime_error("No states in trajectory");

  // We pause at the end and before the beginning
  if (t <= times_[0])
    return std::make_pair(states_[0], controls_[0]);
  else if (t >= times_.back()) 
    return std::make_pair(states_.back(), controls_.back());
  else {
    unsigned int idx = find_closest_t_(t);

    assert(idx != times_.size() - 1);

    double prev_time = times_[idx];
    double next_time = times_[idx+1];
    assert(t >= prev_time && t <= next_time);

    double diff = next_time - prev_time;
    double prog = t - prev_time;
    double x = prog / diff;

    // We do zeroth-order hold control
    return std::make_pair(lerp(states_[idx], states_[idx+1], x), controls_[idx]);
  }

}

void ControlledTrajectory::push_back(const Ref<const VectorXd> &state,
                                     const Ref<const VectorXd> &control,
                                     double time) {
  if (!times_.empty()) {
    if (time < times_.back())
      throw std::runtime_error("Time passed to trajectory is decreasing");

    if (time < 0.0)
      throw std::runtime_error("Times should be >= 0");
  }

  states_.push_back(state);
  controls_.push_back(control);
  times_.push_back(time);
}

double ControlledTrajectory::length() const {
  if (times_.empty())
    return 0.0;
  else
    return times_.back();
}

size_t ControlledTrajectory::size() const {
  return times_.size();
}

unsigned int ControlledTrajectory::find_closest_t_(double t) const {
  auto it = std::upper_bound(times_.begin(), times_.end(), t); 
  
  if (it == times_.end())
    return times_.size() - 1;
  else {
    unsigned int idx = std::max(int(it - times_.begin()) - 1, 0);
    return idx;
  }
}
