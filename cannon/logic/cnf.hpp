#ifndef CANNON_LOGIC_CNF_H
#define CANNON_LOGIC_CNF_H 

#include <set>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <valarray>
#include <cmath>
#include <algorithm>
#include <random>

#include <Eigen/Dense>

#include <cannon/log/registry.hpp>

using namespace Eigen;

using namespace cannon::log;

namespace cannon {
  namespace logic {

    enum class PropAssignment {
      False,
      Unassigned,
      True
    };

    using Assignment=std::valarray<PropAssignment>;
    using Simplification=std::valarray<bool>;

    class Literal {
      public:
        Literal() = delete;

        Literal(unsigned int prop_num, bool negated) : prop_(prop_num), negated_(negated) {}

        Literal(const Literal& l) : prop_(l.prop_), negated_(l.negated_) {}
        
        Literal(Literal&& l) : prop_(l.prop_), negated_(l.negated_) {}

        PropAssignment eval(const Assignment& assignment) const;


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

        unsigned int prop_;
        bool negated_;
    };

    class Clause {
      public:
        void add_literal(Literal l);
        void add_literal(unsigned int prop_num, bool negated);

        unsigned int size() const;
        unsigned int size(const Assignment& a) const;
        bool is_unit() const;
        bool is_unit(const Assignment& a) const;

        std::set<unsigned int> get_props(const Assignment& a);
        bool contains_prop(const Assignment& a, unsigned int prop) const;
        PropAssignment eval(const Assignment& assignment) const;

        PropAssignment get_assignment_for_literal(unsigned int prop);

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

        std::set<Literal> literals_;

      private:
        unsigned int num_props_ = 0;
    };

    class CNFFormula {
      public:
        void add_clause(Clause c);

        PropAssignment eval(const Assignment& assignment,
            const Simplification& s) const;

        std::vector<std::tuple<unsigned int, bool, int>> get_unit_clause_props(const
            Assignment& a, const Simplification& s) const;
        std::vector<std::tuple<unsigned int, bool, int>> get_unit_clause_props(const
            Assignment& a, const Simplification& s,
            std::vector<std::vector<unsigned int>> watched) const;
        std::vector<std::tuple<unsigned int, bool, int>> get_unit_clause_props(const
            Assignment& a, const Simplification& s, std::vector<unsigned int>
            unit_clauses) const;

        unsigned int get_num_props() const;
        unsigned int get_num_clauses() const;
        unsigned int get_num_two_clauses(const Assignment& a, const
            Simplification& s, unsigned int prop) const;
        std::vector<unsigned int> get_num_two_clauses(const Assignment& a, const
            Simplification& s, const std::vector<unsigned int>& props) const;
        std::vector<unsigned int> get_props(const Assignment& a,
            const Simplification& s);
        std::vector<unsigned int> get_props(std::vector<bool> s);

        MatrixXd make_adjacency_mat(const Assignment& a, const Simplification& s) const;

        std::multiset<unsigned int> get_props_multiset(const Assignment& a,
            const Simplification& s);

        PropAssignment is_pure_literal(const Assignment& a, const Simplification& s,
            unsigned int prop);

        Simplification simplify(const Assignment& a,
            const Simplification& s) const;

        bool operator==(const CNFFormula f) const {
          for (auto& c : clauses_) {
            if (std::find(f.clauses_.begin(), f.clauses_.end(), c) == f.clauses_.end()) {
              return false;
            }
          }
          
          return true;
        }

        friend std::ostream& operator<<(std::ostream& os, const CNFFormula& f);

        std::vector<Clause> clauses_;

      private:
        unsigned int num_props_ = 0;
    };

    Clause generate_random_clause(unsigned int num_props);
    CNFFormula generate_random_formula(unsigned int num_props, unsigned int num_clauses);

    Clause resolve(const Clause& c1, const Clause& c2, unsigned int prop);

    std::ostream& operator<<(std::ostream& os, const PropAssignment& a);
    std::ostream& operator<<(std::ostream& os, const Literal& l);
    std::ostream& operator<<(std::ostream& os, const Clause& c);
    std::ostream& operator<<(std::ostream& os, const CNFFormula& f);

    std::ostream& operator<<(std::ostream& os, const std::valarray<PropAssignment>& v);
    bool operator==(const std::valarray<PropAssignment>& v1, const std::valarray<PropAssignment>& v2);
    bool operator!=(const std::valarray<PropAssignment>& v1, const std::valarray<PropAssignment>& v2);

  } // namespace logic
} // namespace cannon

#endif /* ifndef CANNON_LOGIC_CNF_H */
