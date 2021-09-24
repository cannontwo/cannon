#ifndef CANNON_GEOM_TRAJECTORY
#define CANNON_GEOM_TRAJECTORY 

/*!
 * \file cannon/geom/trajectory.hpp
 * \brief File containing Trajectory class definition.
 */

#include <vector>

#include <Eigen/Dense>

#include <cannon/utils/class_forward.hpp>

using namespace Eigen;

namespace cannon {

  namespace math {
    CANNON_CLASS_FORWARD(MultiSpline);
  }

  using namespace cannon::math;

  namespace geom {

    /*!
     * \brief Class representing a time-parameterized trajectory. 
     */
    class Trajectory {
      public:
        
        /*!
         * \brief Default constructor.
         */
        Trajectory();

        /*!
         * \brief Constructor taking vector of states and times.
         */
        Trajectory(const std::vector<VectorXd> &states,
                   const std::vector<double> times);

        /*!
         * \brief Get the state of this trajectory at the input time. This
         * trajectory is treated as a piecewise-linear function over time.
         *
         * \param t The time to evaluate the trajectory at.
         *
         * \returns The linearly interpolated state of this trajectory at the
         * input time.
         */
        VectorXd operator()(double t) const;

        /*!
         * \brief Add a new state at the end of this trajectory, at the input
         * time.
         *
         * \param state The state to add.
         * \param time The time for the new state. Should be greater than any
         * time in the trajectory so far.
         */
        void push_back(const Ref<const VectorXd>& state, double time);

        /*!
         * \brief Get the length (total time) of this trajectory.
         *
         * \returns Length of trajectory.
         */
        double length() const;

        /*!
         * \brief Get the size (number of states) in this trajectory.
         *
         * \returns Trajectory size.
         */
        size_t size() const;

        /*!
         * \brief Get a spline that interpolates this trajectory.
         *
         * \returns A spline which smoothly interpolates this trajectory.
         */
        MultiSpline interp() const;

        /*!
         * \brief Save this trajectory to a file.
         *
         * \param path The path to save this trajectory to.
         */
        void save(const std::string& path);

        /*!
         * \brief Load a trajectory from a file. Overwrites any existing points
         * in this trajectory.
         *
         * \param path The path to load a trajectory from.
         */
        void load(const std::string& path);

      private:

        /*!
         * \brief Find the closest idx such that times_[idx] <= t
         *
         * \param t Input value to locate
         *
         * \returns Index of closest time
         */
        unsigned int find_closest_t_(double t) const;

        std::vector<VectorXd> states_; //!< States in trajectory
        std::vector<double> times_; //!< Times for states in trajectory

    };

    /*!
     * \brief Class representing a time-parameterized trajectory with controls. 
     */
    class ControlledTrajectory {
      public:
        
        /*!
         * \brief Default constructor.
         */
        ControlledTrajectory();

        /*!
         * \brief Constructor taking vector of states, vector of controls, and times.
         */
        ControlledTrajectory(const std::vector<VectorXd> &states,
                             const std::vector<VectorXd> &controls,
                             const std::vector<double> times);

        /*!
         * \brief Get the trajectory of states from this controlled trajectory.
         *
         * \returns A trajectory of the states making up this controlled trajectory.
         */
        Trajectory state_traj() const;

        /*!
         * \brief Get the state and control of this controlled trajectory at the
         * input time. This trajectory is treated as a piecewise-linear function
         * over time.
         *
         * \param t The time to evaluate the trajectory at.
         *
         * \returns A pair containing the linearly interpolated state and
         * control of this trajectory at the input time.
         */
        std::pair<VectorXd, VectorXd> operator()(double t) const;

        /*!
         * \brief Add a new state and control at the end of this trajectory, at
         * the input time.
         *
         * \param state The state to add.
         * \param control The control to add.
         * \param time The time for the new state. Should be greater than any
         * time in the trajectory so far.
         */
        void push_back(const Ref<const VectorXd> &state,
                       const Ref<const VectorXd> &control, double time);

        /*!
         * \brief Get the length (total time) of this trajectory.
         *
         * \returns Length of trajectory.
         */
        double length() const;

        /*!
         * \brief Get the size (number of states) of this trajectory.
         *
         * \returns Trajectory size.
         */
        size_t size() const;

        /*!
         * \brief Save this trajectory to a file.
         *
         * \param path The path to save this trajectory to.
         */
        void save(const std::string& path);

        /*!
         * \brief Load a trajectory from a file. Overwrites any existing points
         * in this trajectory.
         *
         * \param path The path to load a trajectory from.
         */
        void load(const std::string& path);

      private:

        /*!
         * \brief Find the closest idx such that times_[idx] <= t
         *
         * \param t Input value to locate
         *
         * \returns Index of closest time
         */
        unsigned int find_closest_t_(double t) const;


        std::vector<VectorXd> states_; //!< States in trajectory
        std::vector<VectorXd> controls_; //!< Controls in trajectory
        std::vector<double> times_; //!< Times for states in trajectory

    };

  }
}

#endif /* ifndef CANNON_GEOM_TRAJECTORY */
