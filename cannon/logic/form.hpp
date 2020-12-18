#ifndef CANNON_LOGIC_FORM_H
#define CANNON_LOGIC_FORM_H 

#include <iostream>
#include <vector>
#include <memory>
#include <stack>

#include <cannon/logic/cnf.hpp>

namespace cannon {
  namespace logic {

    // Note below we use not short-circuiting 'or' and 'and' so that we throw
    // correctly if not enough truth values are provided to eval().
    
    class Formula {
      public:
        virtual bool eval(const std::vector<bool>& assignment) const = 0;
        virtual CNFFormula to_cnf(bool negated = false) const = 0;

        virtual ~Formula() {}
    };

    class Atomic : public Formula {
      public:
        Atomic(unsigned idx) : idx(idx) {}

        // May throw if not enough truth values passed
        virtual bool eval(const std::vector<bool>& assignment) const override;
        virtual CNFFormula to_cnf(bool negated = false) const override;

        virtual ~Atomic() {}

        const unsigned idx;
    };

    class Negation : public Formula {
      public:
        Negation(const Formula& formula) : formula(std::make_shared<Negation>(formula)) {}

        Negation(std::shared_ptr<Formula> formula) : formula(formula) {}

        // May throw if not enough truth values passed
        virtual bool eval(const std::vector<bool>& assignment) const override;
        virtual CNFFormula to_cnf(bool negated = false) const override;

        virtual ~Negation() {}

        const std::shared_ptr<Formula> formula;
    };

    class And : public Formula {
      public:
        And(std::shared_ptr<Formula> left, std::shared_ptr<Formula> right) :
          left(left), right(right) {}

        // May throw if not enough truth values passed
        virtual bool eval(const std::vector<bool>& assignment) const override;
        virtual CNFFormula to_cnf(bool negated = false) const override;

        virtual ~And() {}

        const std::shared_ptr<Formula> left;
        const std::shared_ptr<Formula> right;
    };

    class Or : public Formula {
      public:
        Or(std::shared_ptr<Formula> left, std::shared_ptr<Formula> right) :
          left(left), right(right) {}

        // May throw if not enough truth values passed
        virtual bool eval(const std::vector<bool>& assignment) const override;
        virtual CNFFormula to_cnf(bool negated = false) const override;

        virtual ~Or() {}

        const std::shared_ptr<Formula> left;
        const std::shared_ptr<Formula> right;
    };

    class Implies : public Formula {
      public:
        Implies(std::shared_ptr<Formula> left, std::shared_ptr<Formula> right) :
          left(left), right(right) {}

        // May throw if not enough truth values passed
        virtual bool eval(const std::vector<bool>& assignment) const override;
        virtual CNFFormula to_cnf(bool negated = false) const override;

        virtual ~Implies() {}

        const std::shared_ptr<Formula> left;
        const std::shared_ptr<Formula> right;
    };

    class Iff : public Formula {
      public:
        Iff(std::shared_ptr<Formula> left, std::shared_ptr<Formula> right) :
          left(left), right(right) {}

        // May throw if not enough truth values passed
        virtual bool eval(const std::vector<bool>& assignment) const override;
        virtual CNFFormula to_cnf(bool negated = false) const override;

        virtual ~Iff() {}

        const std::shared_ptr<Formula> left;
        const std::shared_ptr<Formula> right;
    };

    // Free functions
    std::shared_ptr<Negation> make_negation(std::shared_ptr<Formula> f);
    std::shared_ptr<And> make_and(std::shared_ptr<Formula> f, std::shared_ptr<Formula> g);
    std::shared_ptr<And> make_and(std::shared_ptr<Formula> f, std::stack<std::shared_ptr<Formula>>& fs);
    std::shared_ptr<Or> make_or(std::shared_ptr<Formula> f, std::shared_ptr<Formula> g);
    std::shared_ptr<Or> make_or(std::shared_ptr<Formula> f, std::stack<std::shared_ptr<Formula>>& fs);
    std::shared_ptr<Implies> make_implies(std::shared_ptr<Formula> f, std::shared_ptr<Formula> g);
    std::shared_ptr<Iff> make_iff(std::shared_ptr<Formula> f, std::shared_ptr<Formula> g);

  } // namespace logic
} // namespace cannon

#endif /* ifndef CANNON_LOGIC_FORM_H */
