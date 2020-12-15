#ifndef CANNON_LOGIC_FORM_H
#define CANNON_LOGIC_FORM_H 

#include <iostream>
#include <vector>

namespace cannon {
  namespace logic {

    // Note below we use not short-circuiting 'or' and 'and' so that we throw
    // correctly if not enough truth values are provided to eval().

    class Atomic {
      public:
        Atomic(unsigned idx) : idx(idx) {}

        // May throw if not enough truth values passed
        bool eval(const std::vector<bool>& assignment) const {
          if (idx >= assignment.size()) {
            throw "Not enough propositions in truth assignment";
          }

          return assignment[idx];
        }

        const unsigned idx;
    };

    template <typename F>
    class Negation {
      public:
        Negation(const F& formula) : formula(formula) {}

        // May throw if not enough truth values passed
        bool eval(const std::vector<bool>& assignment) const {
          return !formula.eval(assignment);
        }

        const F formula;
    };

    template <typename F, typename G>
    class And {
      public:
        And(const F& left, const G& right) : left(left), right(right) {}

        // May throw if not enough truth values passed
        bool eval(const std::vector<bool>& assignment) const {
          return (left.eval(assignment) & right.eval(assignment));
        }

        const F left;
        const G right;
    };

    template <typename F, typename G>
    class Or {
      public:
        Or(const F& left, const G& right) : left(left), right(right) {}

        // May throw if not enough truth values passed
        bool eval(const std::vector<bool>& assignment) const {
          return (left.eval(assignment) | right.eval(assignment));
        }

        const F left;
        const G right;
    };

    template <typename F, typename G>
    class Implies {
      public:
        Implies(const F& left, const G& right) : left(left), right(right) {}

        // May throw if not enough truth values passed
        bool eval(const std::vector<bool>& assignment) const {
          return (!left.eval(assignment) | right.eval(assignment));
        }

        const F left;
        const G right;
    };

    template <typename F, typename G>
    class Iff {
      public:
        Iff(const F& left, const G& right) : left(left), right(right) {}

        // May throw if not enough truth values passed
        bool eval(const std::vector<bool>& assignment) const {
          return ((left.eval(assignment) & right.eval(assignment)) | (!left.eval(assignment) & !right.eval(assignment)));
        }

        const F left;
        const G right;
    };

    // Free functions
    template <typename F>
    Negation<F> make_negation(const F& f) {
      return Negation<F>(f);
    }

    template <typename F, typename G>
    And<F, G> make_and(const F& f, const G& g) {
      return And<F, G>(f, g);
    }

    template <typename F, typename G>
    Or<F, G> make_or(const F& f, const G& g) {
      return Or<F, G>(f, g);
    }

    template <typename F, typename G>
    Implies<F, G> make_implies(const F& f, const G& g) {
      return Implies<F, G>(f, g);
    }

    template <typename F, typename G>
    Iff<F, G> make_iff(const F& f, const G& g) {
      return Iff<F, G>(f, g);
    }

    // Operator overloading
    std::ostream& operator<<(std::ostream& os, const Atomic& f) {
      os << "p" << f.idx;

      return os;
    }

    template <typename F>
    std::ostream& operator<<(std::ostream& os, const Negation<F>& f) {
      os << "(!" << f.formula << ")";

      return os;
    }

    template <typename F, typename G>
    std::ostream& operator<<(std::ostream& os, const And<F, G>& f) {
      os << "(" << f.left << "^" << f.right << ")";

      return os;
    }

    template <typename F, typename G>
    std::ostream& operator<<(std::ostream& os, const Or<F, G>& f) {
      os << "(" << f.left << "v" << f.right << ")";

      return os;
    }

    template <typename F, typename G>
    std::ostream& operator<<(std::ostream& os, const Implies<F, G>& f) {
      os << "(" << f.left << ">" << f.right << ")";

      return os;
    }

    template <typename F, typename G>
    std::ostream& operator<<(std::ostream& os, const Iff<F, G>& f) {
      os << "(" << f.left << "-" << f.right << ")";

      return os;
    }

  } // namespace logic
} // namespace cannon

#endif /* ifndef CANNON_LOGIC_FORM_H */
