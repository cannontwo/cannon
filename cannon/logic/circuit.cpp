#include <cannon/logic/circuit.hpp>

#include <cannon/logic/cnf.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::logic;
using namespace cannon::log;

CNFFormula NandGate::to_cnf() const {
  CNFFormula f;

  // Tseytin Transformation
  Clause c1;
  c1.add_literal(input_props_[0], false);
  c1.add_literal(output_props_[0], false);
  f.add_clause(std::move(c1));

  Clause c2;
  c2.add_literal(input_props_[1], false);
  c2.add_literal(output_props_[0], false);
  f.add_clause(std::move(c2));

  Clause c3;
  c3.add_literal(input_props_[0], true);
  c3.add_literal(input_props_[1], true);
  c3.add_literal(output_props_[0], true);
  f.add_clause(std::move(c3));

  return f;
}


CNFFormula AndGate::to_cnf() const {
  unsigned int intermediate_prop = prop_alloc_();

  NandGate gate1(input_props_[0], input_props_[1], intermediate_prop);
  NandGate gate2(intermediate_prop, intermediate_prop, output_props_[0]);
  
  auto f = gate1.to_cnf();
  f.merge(gate2.to_cnf());

  return f;
}

CNFFormula OrGate::to_cnf() const {
  unsigned int iprop1 = prop_alloc_();
  unsigned int iprop2 = prop_alloc_();

  NandGate gate1(input_props_[0], input_props_[0], iprop1);
  NandGate gate2(input_props_[1], input_props_[1], iprop2);
  NandGate gate3(iprop1, iprop2, output_props_[0]);

  auto f = gate1.to_cnf();
  f.merge(gate2.to_cnf());
  f.merge(gate3.to_cnf());

  return f;
}

CNFFormula XorGate::to_cnf() const {
  unsigned int iprop1 = prop_alloc_();
  unsigned int iprop2 = prop_alloc_();
  unsigned int iprop3 = prop_alloc_();

  NandGate gate1(input_props_[0], input_props_[1], iprop1);
  NandGate gate2(input_props_[0], iprop1, iprop2);
  NandGate gate3(input_props_[1], iprop1, iprop3);
  NandGate gate4(iprop2, iprop3, output_props_[0]);

  auto f = gate1.to_cnf();
  f.merge(gate2.to_cnf());
  f.merge(gate3.to_cnf());
  f.merge(gate4.to_cnf());

  return f;
}

CNFFormula FullAdder::to_cnf() const {
  unsigned int iprop1 = prop_alloc_();
  unsigned int iprop2 = prop_alloc_();
  unsigned int iprop3 = prop_alloc_();

  XorGate gate1(input_props_[0], input_props_[1], iprop1, prop_alloc_);
  XorGate gate2(iprop1, input_props_[2], output_props_[0], prop_alloc_);
  AndGate gate3(input_props_[0], input_props_[1], iprop2, prop_alloc_);
  AndGate gate4(iprop1, input_props_[2], iprop3, prop_alloc_);
  OrGate gate5(iprop2, iprop3, output_props_[1], prop_alloc_);

  auto f = gate1.to_cnf();
  f.merge(gate2.to_cnf());
  f.merge(gate3.to_cnf());
  f.merge(gate4.to_cnf());
  f.merge(gate5.to_cnf());

  return f;
}

CNFFormula NBitAdder::to_cnf() const {
  unsigned int iprop = prop_alloc_();

  FullAdder a1(input_props_[0], input_props_[1], input_props_[2*n], 
               output_props_[0], iprop, prop_alloc_);
  auto f = a1.to_cnf();

  for (unsigned int i = 1; i < n - 1; i++) {
    unsigned int new_iprop = prop_alloc_();
    FullAdder a(input_props_[2*i], input_props_[2*i + 1], iprop,
        output_props_[i], new_iprop, prop_alloc_);

    iprop = new_iprop;
    f.merge(a.to_cnf());
  }

  FullAdder af(input_props_[2*n - 2], input_props_[2*n - 1], iprop,
      output_props_[n-1], output_props_[n], prop_alloc_);
  f.merge(af.to_cnf());
  
  return f;
}
