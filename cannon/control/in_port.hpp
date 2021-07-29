#ifndef CANNON_CONTROL_IN_PORT_H
#define CANNON_CONTROL_IN_PORT_H 

/*!
 * \file cannon/control/in_port.hpp
 * \brief File containing InPort class definition.
 */

#include <cannon/utils/class_forward.hpp>

namespace cannon {
  namespace control {

    CANNON_CLASS_FORWARD(Signal);

    /*!
     * \brief Class representing an input port in a control block. Loose
     * metaphor to input ports in Matlab Simulink.
     */
    class InPort {
      public:

        /*!
         * \brief Constructor which does not initialize signal
         */
        InPort() : signal_(nullptr) {}

        /*!
         * \brief Destructor.
         */
        ~InPort() = default;

        /*!
         * \brief Constructor which initializes managed signal
         */
        InPort(SignalPtr s) : signal_(s) {}

        /*!
         * \brief Copy constructor.
         */
        InPort(const InPort& o) : signal_(o.signal_) {}

        /*!
         * \brief Move constructor.
         */
        InPort(InPort& o) : signal_(o.signal_) {
          o.signal_ = nullptr;
        }

        /*!
         * \brief Check that this input port is valid for simulation, i.e., is
         * connected to a signal.
         *
         * \returns Whether the input port is connected to a signal.
         */
        bool is_valid() {
          return signal_ != nullptr;
        }

        /*!
         * \brief Get the signal managed by this input port.
         *
         * \returns Pointer to the signal.
         */
        const SignalPtr signal() const {
          return signal_;
        }

        /*!
         * \brief Connect this input port to a signal.
         *
         * \param s The signal to connect to.
         */
        void connect(SignalPtr s) {
          signal_ = s;
        }

        /*!
         * \brief Get the current value of this port.
         *
         * TODO : Both Ports and Signals should eventually be
         * type-parameterized to extend beyond SISO systems.
         *
         * \returns The current value of the port.
         */
        double value() const;

      private:
        SignalPtr signal_; //!< Signal managed by this input port.

    };

  }
}

#endif /* ifndef CANNON_CONTROL_IN_PORT_H */
