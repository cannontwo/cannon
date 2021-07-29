#ifndef CANNON_CONTROL_SIGNAL_H
#define CANNON_CONTROL_SIGNAL_H 

/*!
 * \file cannon/control/signal.hpp
 * \brief File containing Signal class definition.
 */

namespace cannon {
  namespace control {

    /*!
     * \brief Class representing a signal wire between one output port and one
     * or more input ports. Loose metaphor to block wires in Matlab Simulink.
     */
    class Signal {
      public:

        /*!
         * \brief Sample value from this signal.
         *
         * \returns Current sampled value on this signal.
         */
        double sample() const {
          return value_;
        }

        /*!
         * \brief Set the held value on this signal.
         *
         * \param val The new value for this signal.
         */
        void set_value(double val) {
          value_ = val;
        }

      private:
        // TODO : Both Ports and Signals should eventually be
        // type-parameterized to extend beyond SISO systems.
        double value_ = 0.0; //!< Current value passed along this signal

    };

  }
}

#endif /* ifndef CANNON_CONTROL_SIGNAL_H */
