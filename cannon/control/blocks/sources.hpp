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
          virtual void update() override;

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

    }
  }
}

#endif /* ifndef CANNON_CONTROL_BLOCKS_SOURCES_H */
