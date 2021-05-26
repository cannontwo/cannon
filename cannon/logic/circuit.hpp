#ifndef CANNON_LOGIC_CIRCUIT_H
#define CANNON_LOGIC_CIRCUIT_H 

/*!
 * \file cannon/logic/circuit.hpp
 * \brief File containing class definitions for logical circuits convertible to
 * composable CNF formulas.
 */

/*!
 * \namespace cannon::logic
 * \brief Namespace containing classes representing CNF formulas and tooling
 * for solving CNF formulas.
 */

#include <vector>
#include <functional>

#include <cannon/utils/class_forward.hpp>

namespace cannon {
  namespace logic {

    CANNON_CLASS_FORWARD(CNFFormula);

    /*!
     * \brief Abstract class serving as the basis for other circuits. Circuits
     * can be composed by connecting input and output propositions.
     */
    struct Circuit {

      /*!
       * \brief Get propositions representing outputs of this circuit.
       *
       * \returns Vector of output propositions.
       */
      virtual std::vector<unsigned int> get_output_props() const {
        return output_props_;
      }

      /*!
       * \brief Get propositions representing inputs to this circuit.
       *
       * \returns Vector of input propositions.
       */
      virtual std::vector<unsigned int> get_input_props() const {
        return input_props_;
      }

      /*!
       * \brief Get the CNF formula representing this circuit.
       *
       * \returns The CNF representation.
       */
      virtual CNFFormula to_cnf() const = 0;

      /*!
       * \brief Destructor.
       */
      virtual ~Circuit() {};

      protected:
        std::vector<unsigned int> input_props_; //!< Input propositions
        std::vector<unsigned int> output_props_; //!< Output proposition

    };

    /*!
     * \brief Class representing a NAND gate convertible to CNF.
     */
    struct NandGate : public Circuit {
      NandGate() = delete;

      /*!
       * \brief Constructor taking input and output propositions for this gate.
       */
      NandGate(unsigned int in1, unsigned int in2, unsigned int out) {
        input_props_.push_back(in1);
        input_props_.push_back(in2);

        output_props_.push_back(out);
      }

      /*!
       * \brief Constructor taking two input circuits and an output
       * proposition. This constructor is only applicable for input circuits
       * which each have a single output.
       */
      NandGate(const Circuit& c1, const Circuit& c2, unsigned int out) {
        if ((c1.get_output_props().size() != 1) || (c2.get_output_props().size() != 1))
          throw std::runtime_error("Passed input circuit has more than one output");

        input_props_.push_back(c1.get_output_props()[0]);
        input_props_.push_back(c2.get_output_props()[0]);

        output_props_.push_back(out);
      }

      /*!
       * \brief Inherited from Circuit.
       */
      virtual CNFFormula to_cnf() const;

      /*!
       * \brief Destructor.
       */
      virtual ~NandGate() {}

    };

    /*!
     * \brief Class representing an AND gate covertable to CNF.
     */
    struct AndGate : public Circuit {
      AndGate() = delete;

      /*!
       * \brief Constructor taking two input propositions and an output
       * proposition, as well as a function that allocates propositions for
       * internal usage.
       */
      AndGate(unsigned int in1, unsigned int in2, unsigned int out,
          std::function<unsigned int()> prop_alloc) : prop_alloc_(prop_alloc) {
        input_props_.push_back(in1);
        input_props_.push_back(in2);

        output_props_.push_back(out);
      }

      /*!
       * \brief Constructor taking two input circuits and an output
       * proposition, as well as a function that allocates propositions for
       * internal usage. Note that this constructor is only applicable if both
       * input circuits have only a single output proposition each.
       */
      AndGate(const Circuit& c1, const Circuit& c2, unsigned int out,
          std::function<unsigned int()> prop_alloc) : prop_alloc_(prop_alloc) {
        if ((c1.get_output_props().size() != 1) || (c2.get_output_props().size() != 1))
          throw std::runtime_error("Passed input circuit has more than one output");

        input_props_.push_back(c1.get_output_props()[0]);
        input_props_.push_back(c2.get_output_props()[0]);

        output_props_.push_back(out);
      }

      /*!
       * \brief Inherited from Circuit.
       */
      virtual CNFFormula to_cnf() const;

      /*!
       * \brief Destructor.
       */
      virtual ~AndGate() {}

      std::function<unsigned int()> prop_alloc_; //!< Function used to allocate intermediate propositions.
    };

    /*!
     * \brief Class representing an OR gate convertible to CNF.
     */
    struct OrGate : public Circuit {
      OrGate() = delete;

      /*!
       * \brief Constructor taking two input propositions and an output
       * proposition, as well as a function that allocates propositions for
       * internal usage.
       */
      OrGate(unsigned int in1, unsigned int in2, unsigned int out,
          std::function<unsigned int()> prop_alloc) : prop_alloc_(prop_alloc) {
        input_props_.push_back(in1);
        input_props_.push_back(in2);

        output_props_.push_back(out);
      }

      /*!
       * \brief Constructor taking two input circuits and an output
       * proposition, as well as a function that allocates propositions for
       * internal usage. Note that this constructor is only applicable if both
       * input circuits have only a single output proposition each.
       */
      OrGate(const Circuit& c1, const Circuit& c2, unsigned int out,
          std::function<unsigned int()> prop_alloc) : prop_alloc_(prop_alloc) {
        if ((c1.get_output_props().size() != 1) || (c2.get_output_props().size() != 1))
          throw std::runtime_error("Passed input circuit has more than one output");

        input_props_.push_back(c1.get_output_props()[0]);
        input_props_.push_back(c2.get_output_props()[0]);

        output_props_.push_back(out);
      }

      /*!
       * \brief Inherited from Circuit.
       */
      virtual CNFFormula to_cnf() const;

      /*!
       * \brief Destructor.
       */
      virtual ~OrGate() {}

      std::function<unsigned int()> prop_alloc_; //!< Function used to allocate intermediate propositions.
    };

    /*!
     * \brief Class representing an XOR gate convertible to CNF.
     */
    struct XorGate : public Circuit {
      XorGate() = delete;

      /*!
       * \brief Constructor taking two input propositions and an output
       * proposition, as well as a function that allocates propositions for
       * internal usage.
       */
      XorGate(unsigned int in1, unsigned int in2, unsigned int out,
          std::function<unsigned int()> prop_alloc) : prop_alloc_(prop_alloc) {
        input_props_.push_back(in1);
        input_props_.push_back(in2);

        output_props_.push_back(out);
      }

      /*!
       * \brief Constructor taking two input circuits and an output
       * proposition, as well as a function that allocates propositions for
       * internal usage. Note that this constructor is only applicable if both
       * input circuits have only a single output proposition each.
       */
      XorGate(const Circuit& c1, const Circuit& c2, unsigned int out,
          std::function<unsigned int()> prop_alloc) : prop_alloc_(prop_alloc) {
        if ((c1.get_output_props().size() != 1) || (c2.get_output_props().size() != 1))
          throw std::runtime_error("Passed input circuit has more than one output");

        input_props_.push_back(c1.get_output_props()[0]);
        input_props_.push_back(c2.get_output_props()[0]);

        output_props_.push_back(out);
      }

      /*!
       * \brief Inherited from Circuit.
       */
      virtual CNFFormula to_cnf() const;

      /*!
       * \brief Destructor.
       */
      virtual ~XorGate() {}

      std::function<unsigned int()> prop_alloc_; //!< Function used to allocate intermediate propositions.
    };

    /*!
     * \brief Class representing a full adder convertible to CNF. See
     * https://en.wikipedia.org/wiki/Adder_(electronics)
     */
    struct FullAdder : public Circuit {
      FullAdder() = delete;

      /*!
       * \brief Constructor taking two input propositions, a carry bit
       * proposition, an output proposition, a carry out proposition, and a
       * function used to allocate intermediate propositions.
       */
      FullAdder(unsigned int in1, unsigned int in2, unsigned int carry_in,
          unsigned int out, unsigned int carry_out, 
          std::function<unsigned int()> prop_alloc) : prop_alloc_(prop_alloc) {
        input_props_.push_back(in1);
        input_props_.push_back(in2);
        input_props_.push_back(carry_in);

        output_props_.push_back(out);
        output_props_.push_back(carry_out);
      }

      /*!
       * \brief Inherited from Circuit.
       */
      virtual CNFFormula to_cnf() const;

      /*!
       * \brief Destructor.
       */
      virtual ~FullAdder() {}

      std::function<unsigned int()> prop_alloc_; //!< Function used to allocate intermediate propositions.
    };

    /*!
     * \brief Class representing an Adder for an arbitrary number of bits,
     * convertible to CNF.
     */
    struct NBitAdder : public Circuit {
      NBitAdder() = delete;

      /*!
       * \brief Constructor taking propositions for two input numbers, a carry
       * in bit, an output number, a carry out bit, and a function to allocate
       * intermediate propositions.
       */
      NBitAdder(const std::vector<unsigned int>& in_bits1, const
          std::vector<unsigned int>& in_bits2, unsigned int carry_in, const
          std::vector<unsigned int>& out_bits, unsigned int carry_out,
          std::function<unsigned int()> prop_alloc) : prop_alloc_(prop_alloc) {

        if (in_bits1.size() != out_bits.size())
          throw std::runtime_error("Number of in and out bits does not match");

        if (in_bits2.size() != out_bits.size())
          throw std::runtime_error("Number of in and out bits does not match");

        if (in_bits1.size() < 2)
          throw std::runtime_error("Too few bits passed");

        n = in_bits1.size();

        for (unsigned int i = 0; i < n; i++) {
          input_props_.push_back(in_bits1[i]);
          input_props_.push_back(in_bits2[i]);
        }
        input_props_.push_back(carry_in);
        output_props_ = out_bits;
        output_props_.push_back(carry_out);
      }

      /*!
       * \brief Constructor taking the number of bits for this adder and a
       * function to allocate propositions.
       */
      NBitAdder(unsigned int n, std::function<unsigned int()> prop_alloc) :
        prop_alloc_(prop_alloc), n(n) {

        for (unsigned int i = 0; i < n; i++) {
          input_props_.push_back(prop_alloc_());
          input_props_.push_back(prop_alloc_());
          output_props_.push_back(prop_alloc_());
        }

        // Carry bits
        input_props_.push_back(prop_alloc_());
        output_props_.push_back(prop_alloc_());
      }

      /*!
       * \brief Inherited from Circuit.
       */
      virtual CNFFormula to_cnf() const;

      /*!
       * \brief Destructor.
       */
      virtual ~NBitAdder() {}

      std::function<unsigned int()> prop_alloc_; //!< Function used to allocate intermediate propositions.
      unsigned int n; //!< Number of bits in this adder.
    };

  } // namespace logic
} // namespace cannon

#endif /* ifndef CANNON_LOGIC_CIRCUIT_H */
