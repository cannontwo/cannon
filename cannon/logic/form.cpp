#include <cannon/logic/form.hpp>

using namespace cannon::logic;

bool Atomic::eval(const std::vector<bool>& assignment) const {
  if (idx >= assignment.size()) {
    throw "Not enough propositions in truth assignment";
  }

  return assignment[idx];
}

CNFFormula Atomic::to_cnf(bool negated) const {
  CNFFormula ret_form;
  Clause c;

  c.add_literal(idx, negated);
  ret_form.add_clause(std::move(c));

  return ret_form; 
}

bool Negation::eval(const std::vector<bool>& assignment) const {
  return !formula->eval(assignment);
}

CNFFormula Negation::to_cnf(bool negated) const {
  return formula->to_cnf(!negated); 
}

bool And::eval(const std::vector<bool>& assignment) const {
  return (left->eval(assignment) & right->eval(assignment));
}

CNFFormula And::to_cnf(bool negated) const {
  if (!negated) {
    CNFFormula f1 = left->to_cnf();
    CNFFormula f2 = right->to_cnf();
    CNFFormula ret_form;

    for (auto c : f1.clauses_) {
      ret_form.add_clause(std::move(c));
    }

    for (auto c : f2.clauses_) {
      ret_form.add_clause(std::move(c));
    }

    return ret_form;
  } else {
    return make_or(make_negation(left), make_negation(right))->to_cnf();
  }
}

bool Or::eval(const std::vector<bool>& assignment) const {
  return (left->eval(assignment) || right->eval(assignment));
}

CNFFormula Or::to_cnf(bool negated) const {
  if (!negated) {
    CNFFormula f1 = left->to_cnf();
    CNFFormula f2 = right->to_cnf();
    CNFFormula ret_form;
    
    for (const auto &c1 : f1.clauses_) {
      for (const auto &c2 : f2.clauses_) {
        Clause new_c;
        for (const auto &l : c1.literals_) {
          new_c.add_literal(l);
        }
        for (const auto &l : c2.literals_) {
          new_c.add_literal(l);
        }

        ret_form.add_clause(std::move(new_c));
      }
    }

    return ret_form;
  } else {
    return make_and(make_negation(left), make_negation(right))->to_cnf();
  }
}


bool Implies::eval(const std::vector<bool>& assignment) const {
  return (!left->eval(assignment) || right->eval(assignment));
}

CNFFormula Implies::to_cnf(bool negated) const {
  return make_or(make_negation(left),
      right)->to_cnf(negated);
}

bool Iff::eval(const std::vector<bool>& assignment) const {
  return ((left->eval(assignment) && right->eval(assignment)) ||
      (!left->eval(assignment) && !right->eval(assignment)));
}

CNFFormula Iff::to_cnf(bool negated) const {
  return make_or(make_and(left, right), make_and(make_negation(left),
        make_negation(right)))->to_cnf(negated);
}

// Free Functions
std::shared_ptr<Negation> cannon::logic::make_negation(std::shared_ptr<Formula> f) {
  return std::make_shared<Negation>(f);
}

std::shared_ptr<And> cannon::logic::make_and(std::shared_ptr<Formula> f, std::shared_ptr<Formula> g) {
  return std::make_shared<And>(f, g);
}

std::shared_ptr<Or> cannon::logic::make_or(std::shared_ptr<Formula> f, std::shared_ptr<Formula> g) {
  return std::make_shared<Or>(f, g);
}

std::shared_ptr<Implies> cannon::logic::make_implies(std::shared_ptr<Formula> f, std::shared_ptr<Formula> g) {
  return std::make_shared<Implies>(f, g);
}

std::shared_ptr<Iff> cannon::logic::make_iff(std::shared_ptr<Formula> f, std::shared_ptr<Formula> g) {
  return std::make_shared<Iff>(f, g);
}
