#ifndef CANNON_PHYSICS_LAGRANGIAN_MECHANICS
#define CANNON_PHYSICS_LAGRANGIAN_MECHANICS 

#include <functional>

#include <Eigen/Dense>

#include <cannon/log/registry.hpp>

using namespace Eigen;

using namespace cannon::log;

namespace cannon {
  namespace physics {

    struct LocalTuple {

      LocalTuple() = delete;

      LocalTuple(unsigned int dim) : dim(dim), time(0.0) {
        coordinates = VectorXd::Zero(dim);
        velocities = VectorXd::Zero(dim);
      }

      LocalTuple(const LocalTuple &l) : dim(l.dim), time(l.time),
      coordinates(l.coordinates), velocities(l.velocities) {}

      LocalTuple(LocalTuple &&l) : dim(l.dim), time(l.time),
      coordinates(std::move(l.coordinates)),
      velocities(std::move(l.velocities)) {}

      LocalTuple& operator=(const LocalTuple& o) {
        dim = o.dim;
        time = o.time;
        coordinates = o.coordinates;
        velocities = o.velocities;   

        return *this;
      }

      VectorXd to_vec();

      void from_vec(const VectorXd& vec);

      unsigned int dim;
      double time;
      VectorXd coordinates;
      VectorXd velocities;

    };

    struct Path {
      Path() = delete;

      Path(double t0, double t1, VectorXd start, VectorXd end, 
          unsigned int length) : t0(t0), t1(t1) {
        assert(0 <= t0);
        assert(t0 < t1);
        assert(start.size() == end.size());
        assert(length >= 2);

        VectorXd delta = (end - start) / (double)(length-1);
        
        for (unsigned int i = 0; i < length; i++) {
          waypoints.push_back(start + i * delta); 
        }
      }

      Path(unsigned int dim, const VectorXd& vec) {
        from_vec(dim, vec);
      }

      VectorXd to_vec();

      void from_vec(unsigned int dim, const VectorXd& vec);

      // Just doing linear interpolation for now
      LocalTuple get_local_tuple(double t) const;

      double t0; 
      double t1;
      std::vector<VectorXd> waypoints;
    };

    using Lagrangian = std::function<double(const LocalTuple&)>;

    double compute_path_action(Lagrangian L, const Path& q);

  } // namespace physics
} // namespace cannon

#endif /* ifndef CANNON_PHYSICS_LAGRANGIAN_MECHANICS */
