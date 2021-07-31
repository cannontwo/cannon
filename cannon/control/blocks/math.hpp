#ifndef CANNON_CONTROL_BLOCKS_MATH_H
#define CANNON_CONTROL_BLOCKS_MATH_H 

/*!
 * \file cannon/control/blocks/add.hpp
 * \brief File containing math operations control block class definitions.
 */

/*!
 * \namespace cannon::control::blocks
 * \brief Namespace containing control blocks that can be wired together to
 * model control systems. This uses a similar block diagram metaphor to Matlab
 * Simulink.
 */

#include <cannon/control/block.hpp>

namespace cannon {
  namespace control {

    CANNON_CLASS_FORWARD(InPort);
    CANNON_CLASS_FORWARD(OutPort);
    CANNON_CLASS_FORWARD(Signal);

    namespace blocks {

      /*!
       * \brief Class representing an addition operator block. Adds its two
       * inputs and writes their sum to its output.
       */
      class Add : public Block {
        public:

          /*!
           * Default constructor.
           */
          Add();

          /*!
           * Constructor taking two signals to add.
           */
          Add(SignalPtr in1, SignalPtr in2);

          /*!
           * Inherited from Block.
           */
          virtual std::vector<InPortPtr> in_ports() const override;

          /*!
           * Inherited from Block.
           */
          virtual std::vector<OutPortPtr> out_ports() const override;

          /*!
           * Inherited from Block.
           */
          virtual void update() override;

          /*!
           * Inherited from Block.
           */
          virtual bool is_valid() override;

          /*!
           * \brief Get the single outgoing signal for this block.
           *
           * \returns Pointer to the signal output by this block.
           */
          const SignalPtr out_signal();

        private:
          InPortPtr in1_; //!< First input port
          InPortPtr in2_; //!< Second input port

          OutPortPtr out_; //!< Output port

      };

      /*!
       * Class representing a gain block. Multiplies its input by the stored
       * multiplier to produce output.
       */
      class Gain : public Block {
        public:

          /*!
           * \brief Default constructor.
           */
          Gain();

          /*!
           * \brief Constructor taking multiplier for gain block.
           */
          Gain(double k);

          /*!
           * Inherited from Block.
           */
          virtual std::vector<InPortPtr> in_ports() const override;

          /*!
           * Inherited from Block.
           */
          virtual std::vector<OutPortPtr> out_ports() const override;

          /*!
           * Inherited from Block.
           */
          virtual void update() override;

          /*!
           * Inherited from Block.
           */
          virtual bool is_valid() override;

        private:
          double multiplier_;  //!< Multiplier for this gain block.

          InPortPtr in_; //!< Input port
          OutPortPtr out_; //!< Output port


      };

    }
  }
}

#endif /* ifndef CANNON_CONTROL_BLOCKS_MATH_H */
