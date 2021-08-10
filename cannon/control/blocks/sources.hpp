#ifndef CANNON_CONTROL_BLOCKS_SOURCES_H
#define CANNON_CONTROL_BLOCKS_SOURCES_H 

/*!
 * \file cannon/control/blocks/sources.hpp
 * \brief File containing source block class definitions.
 */

#include <cannon/control/block.hpp>
#include <cannon/utils/class_forward.hpp>

namespace cannon {
  namespace control {

    CANNON_CLASS_FORWARD(Signal);
    CANNON_CLASS_FORWARD(OutPort);

    namespace blocks {

      /*!
       * \brief Class representing a constant-valued source.
       */
      class Constant : public Block {
        public:

          /*!
           * \brief Default constructor.
           */
          Constant();

          /*!
           * \brief Constructor taking a value for this constant block.
           */
          Constant(double value);

          /*!
           * \brief Inherited from Block.
           */
          virtual std::vector<InPortPtr> in_ports() const override;

          /*!
           * \brief Inherited from Block.
           */
          virtual std::vector<OutPortPtr> out_ports() const override;

          /*!
           * \brief Inherited from Block.
           */
          virtual void update(double dt, double t) override;

          /*!
           * \brief Inherited from Block.
           */
          virtual bool is_valid() override;

          /*!
           * \brief Get the value of this constant block.
           *
           * \returns Modifiable reference to the value.
           */
          double& value() {
            return value_;
          }

          /*!
           * \brief Get the single outgoing signal for this block.
           *
           * \returns Pointer to the signal output by this block.
           */
          const SignalPtr out_signal();

        private:
          double value_; //!< Value output by this constant block

          OutPortPtr out_; //!< Out port for this block.
      };

      /*!
       * \brief Class representing a step from one value to another at a specified time.
       */
      class Step : public Block {
        public:

          /*!
           * \brief Default constructor.
           */
          Step();

          /*!
           * \brief Constructor taking an initial value, a final value, and the
           * step time.
           */
          Step(double initial_value, double final_value, double step_time);

          /*!
           * \brief Inherited from Block.
           */
          virtual std::vector<InPortPtr> in_ports() const override;

          /*!
           * \brief Inherited from Block.
           */
          virtual std::vector<OutPortPtr> out_ports() const override;

          /*!
           * \brief Inherited from Block.
           */
          virtual void update(double dt, double t) override;

          /*!
           * \brief Inherited from Block.
           */
          virtual bool is_valid() override;

          /*!
           * \brief Get the single outgoing signal for this block.
           *
           * \returns Pointer to the signal output by this block.
           */
          const SignalPtr out_signal();

        private:
          double initial_value_; //!< Initial value for step
          double final_value_; //!< Final value for step
          double step_time_; //!< Step time

          OutPortPtr out_; //!< Out port for this block.
      };

    }
  }
}

#endif /* ifndef CANNON_CONTROL_BLOCKS_SOURCES_H */
