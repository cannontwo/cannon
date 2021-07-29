#ifndef CANNON_CONTROL_OUT_PORT_H
#define CANNON_CONTROL_OUT_PORT_H 

/*!
 * \file cannon/control/out_port.hpp
 * \brief File containing OutPort class definition.
 */

#include <cannon/utils/class_forward.hpp>

namespace cannon {
  namespace control {

    CANNON_CLASS_FORWARD(Signal);

    /*!
     * \brief Class representing an input port in a control block. Loose
     * metaphor to input ports in Matlab Simulink.
     */
    class OutPort {
      public:

        /*!
         * \brief Constructor which does not initialize signal
         */
        OutPort();

        /*!
         * \brief Destructor.
         */
        ~OutPort() = default;

        // Deleted because each OutPort defines a new Signal
        OutPort(const OutPort& o) = delete;

        /*!
         * \brief Move constructor.
         */
        OutPort(OutPort& o) : signal_(o.signal_) {
          o.signal_ = nullptr;
        }

        /*!
         * \brief Check that this output port is valid for simulation, i.e., is
         * connected to a signal.
         *
         * \returns Whether the input port is connected to a signal.
         */
        bool is_valid() {
          return signal_ != nullptr;
        }

        /*!
         * \brief Get the signal managed by this output port.
         *
         * \returns Pointer to the signal.
         */
        const SignalPtr signal() const {
          return signal_;
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

        /*!
         * \brief Set the current value of this port.
         *
         * TODO : Both Ports and Signals should eventually be
         * type-parameterized to extend beyond SISO systems.
         *
         * \param val The new value for this output port.
         */
        void set_value(double val);

      private:
        SignalPtr signal_; //!< Signal managed by this output port.

    };

  }
}

#endif /* ifndef CANNON_CONTROL_OUT_PORT_H */
