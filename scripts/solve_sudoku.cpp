#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

#include <cannon/log/registry.hpp>
#include <cannon/logic/form.hpp>
#include <cannon/logic/dpll.hpp>

using namespace cannon::log;
using namespace cannon::logic;

unsigned int pos_val_to_prop(unsigned int row, unsigned int col, unsigned int value) {
  assert(row < 9);
  assert(col < 9);
  assert((value > 0) && (value < 10)); 

  return 81*row + 9*col + (value - 1);
}

std::tuple<unsigned int, unsigned int, unsigned int> prop_to_pos_val(unsigned int prop) {
  assert(prop < (9 * 9 * 9));

  unsigned int row = prop / 81;
  unsigned int col = (prop - (row * 81)) / 9;
  unsigned int value = (prop % 9) + 1;

  assert(row < 9);
  assert(col < 9);
  assert((value > 0) && (value < 10)); 

  return std::make_tuple(row, col, value);
}

void add_pos_constraints(CNFFormula& f) {
  for (unsigned int i = 0; i < 9; i++) {
    for (unsigned int j = 0; j < 9; j++) {
      std::stack<std::shared_ptr<Formula>> constraint_stack;
      for (unsigned int k = 2; k < 10; k++) {
        constraint_stack.push(std::make_shared<Atomic>(pos_val_to_prop(i, j, k)));
      }

      auto c = make_or(std::make_shared<Atomic>(pos_val_to_prop(i, j, 1)), constraint_stack);
      f.merge(c->to_cnf());

      for (unsigned int k1 = 1; k1 < 10; k1++) {
        for (unsigned int k2 = k1+1; k2 < 10; k2++) {
          auto c = make_implies(std::make_shared<Atomic>(pos_val_to_prop(i, j, k1)),
              make_negation(std::make_shared<Atomic>(pos_val_to_prop(i, j, k2))));
          f.merge(c->to_cnf());
        }
      }
    }
  }
}

void add_row_constraints(CNFFormula& f) {
  for (unsigned int j = 0; j < 9; j++) {
    for (unsigned int k = 1; k < 10; k++) {
      std::stack<std::shared_ptr<Formula>> constraint_stack;
      for (unsigned int i = 1; i < 9; i++) {
        constraint_stack.push(std::make_shared<Atomic>(pos_val_to_prop(i, j, k)));
      }

      auto c = make_or(std::make_shared<Atomic>(pos_val_to_prop(0, j, k)), constraint_stack);
      f.merge(c->to_cnf());

      for (unsigned i1 = 0; i1 < 9; i1++) {
        for (unsigned i2 = i1+1; i2 < 9; i2++) {
          auto c = make_implies(std::make_shared<Atomic>(pos_val_to_prop(i1, j, k)),
                make_negation(std::make_shared<Atomic>(pos_val_to_prop(i2, j, k))));
          f.merge(c->to_cnf());
        }
      }
    }
  }
}

void add_col_constraints(CNFFormula& f) {
  for (unsigned int i = 0; i < 9; i++) {
    for (unsigned int k = 1; k < 10; k++) {
      std::stack<std::shared_ptr<Formula>> constraint_stack;
      for (unsigned int j = 1; j < 9; j++) {
        constraint_stack.push(std::make_shared<Atomic>(pos_val_to_prop(i, j, k)));
      }

      auto c = make_or(std::make_shared<Atomic>(pos_val_to_prop(i, 0, k)), constraint_stack);
      f.merge(c->to_cnf());

      for (unsigned int j1 = 0; j1 < 9; j1++) {
        for (unsigned int j2 = j1+1; j2 < 9; j2++) {
          auto c = make_implies(std::make_shared<Atomic>(pos_val_to_prop(i, j1, k)),
                make_negation(std::make_shared<Atomic>(pos_val_to_prop(i, j2, k))));
          f.merge(c->to_cnf());
        }
      }
    }
  }
}

void add_box_constraints(CNFFormula& f) {
  for (int offset_i = 0; offset_i < 3; offset_i++) {
    for (int offset_j = 0; offset_j < 3; offset_j++) {

      for (int k = 1; k < 10; k++) {
        std::stack<std::shared_ptr<Formula>> box_k_stack;

        for (int i = 0; i < 3; i++) {
          for (int j = 0; j < 3; j++) {
            box_k_stack.push(std::make_shared<Atomic>(pos_val_to_prop(3*offset_i + i, 3*offset_j + j, k)));

            for (int i2 = 0; i2 < 3; i2++) {
              for (int j2 = 0; j2 < 3; j2++) {
                if ((i != i2) || (j != j2)) {
                  auto c =
                    make_implies(std::make_shared<Atomic>(pos_val_to_prop(3*offset_i
                            + i, 3*offset_j + j, k)),
                        make_negation(std::make_shared<Atomic>(pos_val_to_prop(3*offset_i
                              + i2, 3*offset_j + j2, k))));
                  f.merge(c->to_cnf());

                }
              }
            }
          }
        }
        
        auto first = box_k_stack.top();
        box_k_stack.pop();

        auto c = make_or(first, box_k_stack);
        f.merge(c->to_cnf());
      }

    }
  }
}

CNFFormula make_sudoku_problem() {
  CNFFormula f;

  add_pos_constraints(f);
  add_row_constraints(f);
  add_col_constraints(f);
  add_box_constraints(f);

  return f;
}

void test_prop_assignment() {
  for (unsigned int i = 0; i < 9; i++) {
    for (unsigned int j = 0; j < 9; j++) {
      for (unsigned int k = 1; k < 10; k++) {
        unsigned int prop = pos_val_to_prop(i, j, k);
        log_info("Proposition number for (", i, ",", j, ") =", k, "is", prop);

        unsigned int row, col, val;
        std::tie(row, col, val) = prop_to_pos_val(prop);

        log_info("Recovered (", i, ",", j, ") =", k, "from prop", prop);
        assert(row == i);
        assert(col == j);
        assert(val == k);
      }
    }
  } 
}

void print_sudoku_solution(const Assignment& a) {
  int solution[9][9];

  for (int i = 0; i < 9; i++) {
    for (int j =0; j < 9; j++) {
      solution[i][j] = -1;
    }
  }

  for (unsigned int i = 0; i < a.size(); i++) {
    if (a[i] == PropAssignment::True) {
      unsigned int row, col, val;
      std::tie(row, col, val) = prop_to_pos_val(i);

      assert(solution[row][col] == -1);
      solution[row][col] = val;
    }
  }

  // Print with nice formatting
  for (int i = 0; i < 9; i++) {
    if (i % 3 == 0 && i != 0) {
      std::cout << std::setfill('-') << std::setw(36) << "" << std::endl << std::setfill(' ');
    }

    for (int j = 0; j < 9; j++) {
      if (j % 3 == 0 && j != 0) 
        std:: cout << std::setw(3) << "|";

      std::cout << std::setw(3) << solution[i][j];
    }
    std::cout << std::endl;
  }
}

void set_initial_value(CNFFormula& f, unsigned int row, unsigned int col, unsigned int val) {
  Clause c;
  c.add_literal(pos_val_to_prop(row, col, val), false);
  f.add_clause(std::move(c));
}

int main() {
  // test_prop_assignment();
  auto f = make_sudoku_problem();

  // Example puzzle
  //set_initial_value(f, 0, 1, 2);
  //set_initial_value(f, 1, 3, 6);
  //set_initial_value(f, 1, 8, 3);
  //set_initial_value(f, 2, 1, 7);
  //set_initial_value(f, 2, 2, 4);
  //set_initial_value(f, 2, 4, 8);
  //set_initial_value(f, 3, 5, 3);
  //set_initial_value(f, 3, 8, 2);
  //set_initial_value(f, 4, 1, 8);
  //set_initial_value(f, 4, 4, 4);
  //set_initial_value(f, 4, 7, 1);
  //set_initial_value(f, 5, 0, 6);
  //set_initial_value(f, 5, 3, 5);
  //set_initial_value(f, 6, 4, 1);
  //set_initial_value(f, 6, 6, 7);
  //set_initial_value(f, 6, 7, 8);
  //set_initial_value(f, 7, 0, 5);
  //set_initial_value(f, 7, 5, 9);
  //set_initial_value(f, 8, 7, 4);

  log_info("Enter initial values in grid as '<row> <col> <value>'");
  log_info("Hit Enter on an empty line to run the solver");
  for (std::string line; std::getline(std::cin, line);) {
    if (line.empty()) {
      log_info("Running solver");
      break;
    }

    int row, col, val;
    std::stringstream ss(line);
    ss >> row; 
    ss >> col; 
    ss >> val; 

    log_info("Parsed row =", row, ", col =", col, ", val =", val);
    set_initial_value(f, row, col, val);
  }

  DPLLResult r;
  Assignment a;
  int count;

  std::tie(r, a, count) = dpll(f, std::chrono::seconds(1200));
  log_info("DPLL recursively called", count, "times");
  log_info("DPLL on sudoku problem return result", r, "with assignment");

  log_info("Sudoku solution is:");

  print_sudoku_solution(a);

  Simplification s = std::valarray<bool>(false, f.get_num_clauses());
  if (r == DPLLResult::Satisfiable) {
    log_info("Evaluation of formula on returned assignment is", f.eval(a, s));
  }
}
