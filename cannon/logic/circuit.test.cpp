#include <catch2/catch.hpp>

#include <cannon/logic/circuit.hpp>
#include <cannon/logic/dpll.hpp>

using namespace cannon::logic;

TEST_CASE("Circuit", "[logic]") {
  DPLLResult r;
  Assignment a;
  int c;

  // NAND
  NandGate g1(0, 1, 2);
  auto f = g1.to_cnf();
  auto f2(f);

  f.add_unit_clause(0, false);
  f.add_unit_clause(1, false);

  std::tie(r, a, c) = dpll(f);
  REQUIRE(r == DPLLResult::Satisfiable);
  REQUIRE(a[2] == PropAssignment::False);

  f2.add_unit_clause(0, true);
  f2.add_unit_clause(1, true);
  std::tie(r, a, c) = dpll(f2);
  REQUIRE(r == DPLLResult::Satisfiable);
  REQUIRE(a[2] == PropAssignment::True);

  // AND
  AndGate g2(0, 1, 2, [](){
      static unsigned int i = 2;
      i += 1;
      return i;
      });
  f = g2.to_cnf();
  f2 = f;

  f.add_unit_clause(0, false);
  f.add_unit_clause(1, false);

  std::tie(r, a, c) = dpll(f);
  REQUIRE(r == DPLLResult::Satisfiable);
  REQUIRE(a[2] == PropAssignment::True);
  REQUIRE(a[3] == PropAssignment::False);

  f2.add_unit_clause(0, true);
  f2.add_unit_clause(1, true);

  std::tie(r, a, c) = dpll(f2);
  REQUIRE(r == DPLLResult::Satisfiable);
  REQUIRE(a[2] == PropAssignment::False);
  REQUIRE(a[3] == PropAssignment::True);

  // OR
  OrGate g3(0, 1, 2, [](){
      static unsigned int i = 2;
      i += 1;
      return i;
      });
  f = g3.to_cnf();
  f2 = f;

  f.add_unit_clause(0, false);
  f.add_unit_clause(1, false);

  std::tie(r, a, c) = dpll(f);
  REQUIRE(r == DPLLResult::Satisfiable);
  REQUIRE(a[2] == PropAssignment::True);
  REQUIRE(a[3] == PropAssignment::False);
  REQUIRE(a[4] == PropAssignment::False);

  f2.add_unit_clause(0, true);
  f2.add_unit_clause(1, true);

  std::tie(r, a, c) = dpll(f2);
  REQUIRE(r == DPLLResult::Satisfiable);
  REQUIRE(a[2] == PropAssignment::False);
  REQUIRE(a[3] == PropAssignment::True);
  REQUIRE(a[4] == PropAssignment::True);

  // XOR
  XorGate g4(0, 1, 2, [](){
      static unsigned int i = 2;
      i += 1;
      return i;
      });
  f = g4.to_cnf();
  f2 = f;

  f.add_unit_clause(0, false);
  f.add_unit_clause(1, false);

  std::tie(r, a, c) = dpll(f);
  REQUIRE(r == DPLLResult::Satisfiable);
  REQUIRE(a[2] == PropAssignment::False);
  REQUIRE(a[3] == PropAssignment::False);
  REQUIRE(a[4] == PropAssignment::True);
  REQUIRE(a[5] == PropAssignment::True);

  f2.add_unit_clause(0, true);
  f2.add_unit_clause(1, true);

  std::tie(r, a, c) = dpll(f2);
  REQUIRE(r == DPLLResult::Satisfiable);
  REQUIRE(a[2] == PropAssignment::False);
  REQUIRE(a[3] == PropAssignment::True);
  REQUIRE(a[4] == PropAssignment::True);
  REQUIRE(a[5] == PropAssignment::True);

  // Full Adder
  FullAdder g5(0, 1, 2, 3, 4, [](){
      static unsigned int i = 4;
      i += 1;
      return i;
      });
  f = g5.to_cnf();
  f2 = f;
  auto f3 = f;
  auto f4 = f;

  f.add_unit_clause(0, true);
  f.add_unit_clause(1, true);
  f.add_unit_clause(2, true);

  std::tie(r, a, c) = dpll(f);
  REQUIRE(r == DPLLResult::Satisfiable);
  REQUIRE(a[3] == PropAssignment::False);
  REQUIRE(a[4] == PropAssignment::False);

  f2.add_unit_clause(0, false);
  f2.add_unit_clause(1, false);
  f2.add_unit_clause(2, true);

  std::tie(r, a, c) = dpll(f2);
  REQUIRE(r == DPLLResult::Satisfiable);
  REQUIRE(a[3] == PropAssignment::False);
  REQUIRE(a[4] == PropAssignment::True);

  f3.add_unit_clause(0, false);
  f3.add_unit_clause(1, true);
  f3.add_unit_clause(2, true);

  std::tie(r, a, c) = dpll(f3);
  REQUIRE(r == DPLLResult::Satisfiable);
  REQUIRE(a[3] == PropAssignment::True);
  REQUIRE(a[4] == PropAssignment::False);

  f4.add_unit_clause(0, false);
  f4.add_unit_clause(1, false);
  f4.add_unit_clause(2, false);

  std::tie(r, a, c) = dpll(f4);
  REQUIRE(r == DPLLResult::Satisfiable);
  REQUIRE(a[3] == PropAssignment::True);
  REQUIRE(a[4] == PropAssignment::True);

  // N-Bit Adder
  NBitAdder adder(4, [](){
      static unsigned int i = -1;
      i += 1;
      return i;
      });

  auto in_props = adder.get_input_props();
  auto out_props = adder.get_output_props();

  auto f5 = adder.to_cnf();
  log_info("4-bit adder circuit generated", f5.get_num_clauses(), "CNF clauses with", 
      f5.get_num_props(), "props");

  // Setting in1 to seven = 0111
  f5.add_unit_clause(in_props[0], false);
  f5.add_unit_clause(in_props[2], false);
  f5.add_unit_clause(in_props[4], false);
  f5.add_unit_clause(in_props[6], true);
  f5.add_unit_clause(in_props[8], true); // Carry in

  // Setting in2 to two = 0010
  f5.add_unit_clause(in_props[1], true);
  f5.add_unit_clause(in_props[3], false);
  f5.add_unit_clause(in_props[5], true);
  f5.add_unit_clause(in_props[7], true);

  // Result should be 9 = 1001, with carry=0
  std::tie(r, a, c) = dpll(f5);
  REQUIRE(r == DPLLResult::Satisfiable);
  REQUIRE(a[out_props[0]] == PropAssignment::True);
  REQUIRE(a[out_props[1]] == PropAssignment::False);
  REQUIRE(a[out_props[2]] == PropAssignment::False);
  REQUIRE(a[out_props[3]] == PropAssignment::True);
  REQUIRE(a[out_props[4]] == PropAssignment::False); // Carry out
}
