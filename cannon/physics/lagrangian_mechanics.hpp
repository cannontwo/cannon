#ifndef CANNON_PHYSICS_LAGRANGIAN_MECHANICS
#define CANNON_PHYSICS_LAGRANGIAN_MECHANICS 

/*!
 * \file cannon/physics/lagrangian_mechanics.hpp
 * \brief File containing class and free function definitions for working with
 * Lagrangian dynamics, as in Structure and Interpretation of Classical
 * Mechanics.
 */

#include <functional>

#include <Eigen/Dense>

#include <cannon/log/registry.hpp>

using namespace Eigen;

using namespace cannon::log;

namespace cannon {
  namespace physics {

    /*!
     * \brief Struct representing a local tuple consisting of a coordinate
     * vector, velocity vector, and time. 
     */
    struct LocalTuple {

      LocalTuple() = delete;

      /*!
       * \brief Constructor taking generalized coordinate dimension and
       * initializing coordinates and time to zero.
       */
      LocalTuple(unsigned int dim) : dim(dim), time(0.0) {
        coordinates = VectorXd::Zero(dim);
        velocities = VectorXd::Zero(dim);
      }

      /*!
       * \brief Copy constructor.
       */
      LocalTuple(const LocalTuple &l) : dim(l.dim), time(l.time),
      coordinates(l.coordinates), velocities(l.velocities) {}

      /*!
       * \brief Move constructor.
       */
      LocalTuple(LocalTuple &&l) : dim(l.dim), time(l.time),
      coordinates(std::move(l.coordinates)),
      velocities(std::move(l.velocities)) {}

      /*!
       * \brief Copy assignment operator.
       */
      LocalTuple& operator=(const LocalTuple& o) {
        dim = o.dim;
        time = o.time;
        coordinates = o.coordinates;
        velocities = o.velocities;   

        return *this;
      }

      /*!
       * \brief Get a vector representation of this local tuple.
       *
       * \returns A vector containing the time, generalized coordinates, and
       * generalized velocity for this local tuple, in that order.
       */
      VectorXd to_vec();

      /*!
       * \brief Initialize this local tuple with the input vector, which should
       * have the time, generalized coordinates, and generalized velocities, in
       * that order.
       *
       * \param vec The vector to use for initialization.
       */
      void from_vec(const VectorXd& vec);

      unsigned int dim; //!< Dimension of generalized coordinates
      double time; //!< Time for this local tuple
      VectorXd coordinates; //!< Generalized coordinates for this local tuple.
      VectorXd velocities; //!< Generalized velocities for this local tuple

    };

    /*!
     * \brief Struct representing a path as a series of waypoints in
     * generalized coordinates for doing Lagrangian mechanics.
     */
    struct Path {
      Path() = delete;

      /*!
       * \brief Straight-line constructor taking a start, and end time and
       * start and end coordinates, as well as the number of waypoints to
       * construct.
       */
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

      /*!
       * \brief Constructor taking generalized coordinate dimension and a
       * vector of appended waypoints.
       */
      Path(unsigned int dim, const VectorXd& vec) {
        from_vec(dim, vec);
      }

      /*!
       * \brief Get a vector representation of this path by appending all
       * waypoints.
       *
       * \returns The vector representation.
       */
      VectorXd to_vec();

      /*!
       * \brief Load a path with waypoints of the input dimension from the
       * input vector.
       *
       * \param vec The vector to load.
       */
      void from_vec(unsigned int dim, const VectorXd& vec);

      /*!
       * \brief Get the local tuple at time t from this path. 
       *
       * Just doing linear interpolation for now.
       *
       * \param t The time to get local tuple for.
       *
       * \returns Local tuple at the input time along this path.
       */
      LocalTuple get_local_tuple(double t) const;

      double t0; //!< Start time for this path
      double t1; //!< End time for this path
      std::vector<VectorXd> waypoints; //!< Waypoints making up this path
    };

    using Lagrangian = std::function<double(const LocalTuple&)>;

    /*!
     * \brief Compute path action of the input Lagrangian on the input path.
     *
     * \param L The Lagrangian to use
     * \param q The path to evaluate path action on
     * 
     * \returns The path action
     */
    double compute_path_action(Lagrangian L, const Path& q);

  } // namespace physics
} // namespace cannon

#endif /* ifndef CANNON_PHYSICS_LAGRANGIAN_MECHANICS */
