#ifndef CANNON_LOGIC_CNF_H
#define CANNON_LOGIC_CNF_H 

#include <set>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <valarray>
#include <cmath>
#include <algorithm>

namespace cannon {
  namespace logic {

    enum PropAssignment {
      False,
      Unassigned,
      True
    };

    class Literal {
      public:
        Literal() = delete;

        Literal(unsigned int prop_num, bool negated) : prop_(prop_num), negated_(negated) {}

        Literal(const Literal& l) : prop_(l.prop_), negated_(l.negated_) {}
        
        Literal(Literal&& l) : prop_(l.prop_), negated_(l.negated_) {}

        PropAssignment eval(const std::valarray<PropAssignment> assignment) const;


        bool operator<(const Literal& l) const {
          if (prop_ < l.prop_) {
            return true;
          } else if (prop_ > l.prop_) {
            return false;
          } else {
            return negated_ == l.negated_ ? false : negated_;
          }
        }

        friend std::ostream& operator<<(std::ostream& os, const Literal& l);

        friend class Clause;
        friend class CNFFormula;

      private:
        unsigned int prop_;
        bool negated_;
    };

    class Clause {
      public:
        void add_literal(Literal l);
        void add_literal(unsigned int prop_num, bool negated);

        unsigned int size() const;
        bool is_unit() const;
        bool is_unit(std::valarray<PropAssignment> a) const;

        PropAssignment eval(const std::valarray<PropAssignment> assignment) const;

        friend std::ostream& operator<<(std::ostream& os, const Clause& c);

        friend class CNFFormula;

        bool operator==(const Clause c) const {
          for (auto& l : literals_) {
            if (c.literals_.find(l) == c.literals_.end()) {
              return false;
            }
          }
          
          return true;
        }

      private:
        std::set<Literal> literals_;
        unsigned int num_props_ = 0;
    };

    class CNFFormula {
      public:
        void add_clause(Clause c);

        PropAssignment eval(const std::valarray<PropAssignment> assignment) const;

        std::vector<std::pair<unsigned int, bool>> get_unit_clauses(const
            std::valarray<PropAssignment>& a) const;
        unsigned int get_num_props() const;

        bool operator==(const CNFFormula f) const {
          for (auto& c : clauses_) {
            if (std::find(f.clauses_.begin(), f.clauses_.end(), c) == f.clauses_.end()) {
              return false;
            }
          }
          
          return true;
        }

        friend std::ostream& operator<<(std::ostream& os, const CNFFormula& f);

      private:
        std::vector<Clause> clauses_;
        unsigned int num_props_ = 0;
    };

    std::ostream& operator<<(std::ostream& os, const PropAssignment& a);
    std::ostream& operator<<(std::ostream& os, const Literal& l);
    std::ostream& operator<<(std::ostream& os, const Clause& c);
    std::ostream& operator<<(std::ostream& os, const CNFFormula& f);

  } // namespace logic
} // namespace cannon

#endif /* ifndef CANNON_LOGIC_CNF_H */
