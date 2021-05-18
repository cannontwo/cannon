#ifndef CANNON_LOGIC_CIRCUIT_H
#define CANNON_LOGIC_CIRCUIT_H 

#include <vector>
#include <functional>

#include <cannon/utils/class_forward.hpp>

namespace cannon {
  namespace logic {

    CANNON_CLASS_FORWARD(CNFFormula);

    struct Circuit {

      virtual std::vector<unsigned int> get_output_props() const {
        return output_props_;
      }

      virtual std::vector<unsigned int> get_input_props() const {
        return input_props_;
      }

      virtual CNFFormula to_cnf() const = 0;

      virtual ~Circuit() {};

      protected:
        std::vector<unsigned int> input_props_;
        std::vector<unsigned int> output_props_;

    };

    struct NandGate : public Circuit {
      NandGate() = delete;

      NandGate(unsigned int in1, unsigned int in2, unsigned int out) {
        input_props_.push_back(in1);
        input_props_.push_back(in2);

        output_props_.push_back(out);
      }

      NandGate(const Circuit& c1, const Circuit& c2, unsigned int out) {
        if ((c1.get_output_props().size() != 1) || (c2.get_output_props().size() != 1))
          throw std::runtime_error("Passed input circuit has more than one output");

        input_props_.push_back(c1.get_output_props()[0]);
        input_props_.push_back(c2.get_output_props()[0]);

        output_props_.push_back(out);
      }

      virtual CNFFormula to_cnf() const;

      virtual ~NandGate() {}

    };

    struct AndGate : public Circuit {
      AndGate() = delete;

      AndGate(unsigned int in1, unsigned int in2, unsigned int out,
          std::function<unsigned int()> prop_alloc) : prop_alloc_(prop_alloc) {
        input_props_.push_back(in1);
        input_props_.push_back(in2);

        output_props_.push_back(out);
      }

      AndGate(const Circuit& c1, const Circuit& c2, unsigned int out,
          std::function<unsigned int()> prop_alloc) : prop_alloc_(prop_alloc) {
        if ((c1.get_output_props().size() != 1) || (c2.get_output_props().size() != 1))
          throw std::runtime_error("Passed input circuit has more than one output");

        input_props_.push_back(c1.get_output_props()[0]);
        input_props_.push_back(c2.get_output_props()[0]);

        output_props_.push_back(out);
      }

      virtual CNFFormula to_cnf() const;

      virtual ~AndGate() {}

      std::function<unsigned int()> prop_alloc_;
    };

    struct OrGate : public Circuit {
      OrGate() = delete;

      OrGate(unsigned int in1, unsigned int in2, unsigned int out,
          std::function<unsigned int()> prop_alloc) : prop_alloc_(prop_alloc) {
        input_props_.push_back(in1);
        input_props_.push_back(in2);

        output_props_.push_back(out);
      }

      OrGate(const Circuit& c1, const Circuit& c2, unsigned int out,
          std::function<unsigned int()> prop_alloc) : prop_alloc_(prop_alloc) {
        if ((c1.get_output_props().size() != 1) || (c2.get_output_props().size() != 1))
          throw std::runtime_error("Passed input circuit has more than one output");

        input_props_.push_back(c1.get_output_props()[0]);
        input_props_.push_back(c2.get_output_props()[0]);

        output_props_.push_back(out);
      }

      virtual CNFFormula to_cnf() const;

      virtual ~OrGate() {}

      std::function<unsigned int()> prop_alloc_;
    };

    struct XorGate : public Circuit {
      XorGate() = delete;

      XorGate(unsigned int in1, unsigned int in2, unsigned int out,
          std::function<unsigned int()> prop_alloc) : prop_alloc_(prop_alloc) {
        input_props_.push_back(in1);
        input_props_.push_back(in2);

        output_props_.push_back(out);
      }

      XorGate(const Circuit& c1, const Circuit& c2, unsigned int out,
          std::function<unsigned int()> prop_alloc) : prop_alloc_(prop_alloc) {
        if ((c1.get_output_props().size() != 1) || (c2.get_output_props().size() != 1))
          throw std::runtime_error("Passed input circuit has more than one output");

        input_props_.push_back(c1.get_output_props()[0]);
        input_props_.push_back(c2.get_output_props()[0]);

        output_props_.push_back(out);
      }

      virtual CNFFormula to_cnf() const;

      virtual ~XorGate() {}

      std::function<unsigned int()> prop_alloc_;
    };

    struct FullAdder : public Circuit {
      FullAdder() = delete;

      FullAdder(unsigned int in1, unsigned int in2, unsigned int carry_in,
          unsigned int out, unsigned int carry_out, 
          std::function<unsigned int()> prop_alloc) : prop_alloc_(prop_alloc) {
        input_props_.push_back(in1);
        input_props_.push_back(in2);
        input_props_.push_back(carry_in);

        output_props_.push_back(out);
        output_props_.push_back(carry_out);
      }

      virtual CNFFormula to_cnf() const;

      virtual ~FullAdder() {}

      std::function<unsigned int()> prop_alloc_;
    };

    struct NBitAdder : public Circuit {
      NBitAdder() = delete;

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

      virtual CNFFormula to_cnf() const;

      virtual ~NBitAdder() {}

      std::function<unsigned int()> prop_alloc_;
      unsigned int n;
    };

  } // namespace logic
} // namespace cannon

#endif /* ifndef CANNON_LOGIC_CIRCUIT_H */
