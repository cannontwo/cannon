#include <cannon/physics/lagrangian_mechanics.hpp>

using namespace cannon::physics;

// LocalTuple methods
VectorXd LocalTuple::to_vec() {
  VectorXd ret = VectorXd::Zero(1 + 2*dim);

  ret[0] = time;
  ret.segment(1, dim) = coordinates;
  ret.segment(1+dim, dim) = velocities;

  return ret;
}

void LocalTuple::from_vec(const VectorXd& vec) {
  assert(vec.size() >= 3);
  assert(vec.size() % 2 == 1);

  dim = (vec.size() - 1) / 2;
  time = vec[0];
  coordinates = vec.segment(1, dim);
  velocities = vec.segment(1+dim, dim);
}

// Path methods
LocalTuple Path::get_local_tuple(double t) const {
  assert(t0 <= t);
  assert(t <= t1); // So that we don't exceed the length of the path

  double time_delta = (t1 - t0) / (double)(waypoints.size() - 1);

  if (t == t1) {
    LocalTuple ret(waypoints[0].size());
    ret.time = t;
    ret.coordinates = waypoints[waypoints.size() - 1];
    ret.velocities = (waypoints[waypoints.size() - 1] - waypoints[waypoints.size() - 2]) / time_delta;

    return ret;
  }

  unsigned int prev_idx = std::floor((t - t0) / time_delta);

  VectorXd previous_waypoint = waypoints[prev_idx];
  VectorXd next_waypoint = waypoints[prev_idx + 1];
  double tau = (t - (prev_idx * time_delta)) / time_delta;

  VectorXd interp_coord = previous_waypoint + tau * (next_waypoint - previous_waypoint);
  VectorXd interp_vel = (next_waypoint - previous_waypoint) / time_delta;

  LocalTuple ret(waypoints[0].size());
  ret.time = t;
  ret.coordinates = interp_coord;
  ret.velocities = interp_vel;

  return ret;
}

VectorXd Path::to_vec() {
  VectorXd ret(waypoints.size());
  for (unsigned int i = 0; i < waypoints.size(); i++) {
    ret.segment(i*waypoints[0].size(), waypoints[0].size()) = waypoints[i];
  }

  return ret;
}

void Path::from_vec(unsigned int dim, const VectorXd& vec) {
  assert(vec.size() % dim == 0);
  for (unsigned int i = 0; i < vec.size() / dim; i++) {
    waypoints.push_back(vec.segment(i*dim, dim));
  }
}

// Free Functions
double cannon::physics::compute_path_action(Lagrangian L, const Path& q) {
  // Assuming piecewise-linear path, so trapezoid rule is fine for now.
  // TODO Implement an actual integration algorithm
  
  double coef = (q.t1 - q.t0) / (double)(2 * (q.waypoints.size() - 1));
  double total_action = L(q.get_local_tuple(q.t0));
  total_action += L(q.get_local_tuple(q.t1));

  double time_delta = (q.t1 - q.t0) / (double)(q.waypoints.size() - 1);
  for (unsigned int i = 1; i < q.waypoints.size() - 1; i++) {
    total_action += 2.0 * L(q.get_local_tuple(q.t0 + (time_delta * i)));
  }

  return coef * total_action;
}
