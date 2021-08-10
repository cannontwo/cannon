#ifndef CANNON_CONTROL_BLOCK_H
#define CANNON_CONTROL_BLOCK_H 

/*!
 * \file cannon/control/block.hpp
 * \brief File containing Block class definition.
 */

#include <vector>

#include <cannon/utils/class_forward.hpp>

namespace cannon {
  namespace control {

    CANNON_CLASS_FORWARD(InPort);
    CANNON_CLASS_FORWARD(OutPort);

    /*!
     * \brief Class representing a block in a control block diagram. Loose
     * metaphor to Matlab Simulink blocks.
     */
    class Block {
      public:

        /*!
         * \brief Get the input ports for this block.
         *
         * \returns A vector of input ports.
         */
        virtual std::vector<InPortPtr> in_ports() const = 0;

        /*!
         * \brief Get the output ports for this block.
         *
         * \returns A vector of output ports.
         */
        virtual std::vector<OutPortPtr> out_ports() const = 0;

        /*!
         * \brief Trigger an update on this block. The block should read from
         * its inputs and write to its outputs when triggered in this way.
         *
         * \param dt Time step for update.
         * \param t Total elapsed time so far.
         */
        virtual void update(double dt, double t) = 0;

        /*!
         * \brief Check whether this block is valid for use in simulation,
         * i.e., whether its input ports are correctly wired.
         */
        virtual bool is_valid() = 0;

    };

  }
}

#endif /* ifndef CANNON_CONTROL_BLOCK_H */
