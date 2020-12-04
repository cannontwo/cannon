#ifndef CANNON_LOGIC_DPLL_H
#define CANNON_LOGIC_DPLL_H 

#include <stack>
#include <utility>
#include <cassert>
#include <algorithm>
#include <tuple>
#include <chrono>

#include <cannon/logic/cnf.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::log;

namespace cannon {
  namespace logic {



    using Assignment=std::valarray<PropAssignment>;
    using Simplification=std::valarray<bool>;

    enum class DPLLResult {
      Satisfiable,
      Unknown,
      Unsatisfiable
    };

    // Forward declarations
    template <typename F, typename G>
    class DPLLState;

    template <typename F, typename G>
    std::tuple<DPLLResult, Assignment, int> dpll(CNFFormula f,
        F ph_func, G ah_func, const std::chrono::seconds cutoff=std::chrono::seconds(1200));

    // Heuristic functor for use when choosing propositions
    template <typename F>
    class PropositionHeuristic {
      public:
        PropositionHeuristic() = delete;

        PropositionHeuristic(F f) : f_(f) {}

        std::vector<double> rank_props(const CNFFormula& form, const Assignment& a,
            const Simplification& s, const std::vector<unsigned int>& props) {
          return f_(form, a, s, props);
        }

        unsigned int choose_prop(const CNFFormula& form, const Assignment& a,
            const Simplification& s, const std::vector<unsigned int>& props) {
          auto prop_ranks = rank_props(form, a, s, props);

          unsigned int prop_idx = std::distance(prop_ranks.begin(),
              std::max_element(prop_ranks.begin(), prop_ranks.end()));

          return props[prop_idx];
        }

      private:
        F f_;

    };

    // Heuristic functor for use when applying splitting rule
    template <typename F>
    class AssignmentHeuristic {
      public:
        AssignmentHeuristic() = delete;

        AssignmentHeuristic(F f) : f_(f) {}

        bool choose_assignment(const CNFFormula& form, const Assignment& a, const
            Simplification& s, unsigned int prop) {
          return f_(form, a, s, prop);
        }

      private:
        F f_;
      
    };


    template <typename F, typename G>
    class DPLLState {
      public:

        DPLLState() = delete; 
        
        DPLLState(CNFFormula f, PropositionHeuristic<F> ph,
            AssignmentHeuristic<G> ah) : formula_(f), ph_(ph), ah_(ah) {
          do_preprocessing();
        }

        Assignment do_pure_literal_deletion(const Assignment& a_0, const Simplification& s_0) {
          Assignment a = a_0;

          std::multiset<unsigned int> all_props = formula_.get_props_multiset(a_0, s_0);
          int num_removed_props = 0;
          for (unsigned int i = 0; i < formula_.get_num_props(); i++) {
            int prop_count = all_props.count(i);
            if (prop_count != 0 && a[i] == PropAssignment::Unassigned) {
              a[i] = formula_.is_pure_literal(a, s_0, i);

              if (a[i] != PropAssignment::Unassigned)
                num_removed_props += 1;
            }
          }

          return a;
        }

        void do_preprocessing() {
          Assignment a_0 = Assignment(PropAssignment::Unassigned, formula_.get_num_props());
          Simplification s_0 = Simplification(false, formula_.get_num_clauses());

          Assignment pld_a = do_pure_literal_deletion(a_0, s_0);
          Simplification s = formula_.simplify(pld_a, s_0);

          frontier_.push({pld_a, s}); 
        }

        void do_splitting_rule(Assignment& a, Simplification& s) {
          // Create vector of all propositions
          std::vector<unsigned int> all_props = formula_.get_props(a, s);
          if (all_props.size() == 0) {
            return;
          }

          unsigned int prop = ph_.choose_prop(formula_, a, s, all_props);
          bool first_assignment = ah_.choose_assignment(formula_, a, s, prop);

          assert(a[prop] == PropAssignment::Unassigned);
          //log_info("Splitting on", prop);
          
          Assignment split_first_a(a);
          split_first_a[prop] = first_assignment ? PropAssignment::True : PropAssignment::False;
          Simplification split_first_s = formula_.simplify(split_first_a, s);

          Assignment split_second_a(a);
          split_second_a[prop] = first_assignment ? PropAssignment::False : PropAssignment::True;
          Simplification split_second_s = formula_.simplify(split_second_a, s);

          // Push the second branch to be evaluated first, since we're using a stack
          assert(split_first_a != split_second_a);
          assert(split_first_a != a);
          assert(split_second_a != a);
          frontier_.push({split_second_a, split_second_s});
          frontier_.push({split_first_a, split_first_s});
        }

        void do_unit_preference(Assignment& a, Simplification& s) {
          std::vector<std::pair<unsigned int, bool>> units =
            formula_.get_unit_clause_props(a, s);

          if (units.size() == 0) {
            return;
          }

          std::set<unsigned int> unit_props;
          for (auto& pair : units) {
            unit_props.insert(pair.first);
          }

          std::vector<unsigned int> unit_prop_vec;
          unit_prop_vec.reserve(unit_props.size());
          unit_prop_vec.insert(unit_prop_vec.end(), unit_props.begin(), unit_props.end());

          unsigned int prop = ph_.choose_prop(formula_, a, s, unit_prop_vec);
          //log_info("Doing unit preference on", prop);
          Assignment unit_a(a);

          auto idx = std::find_if(units.begin(), units.end(), 
              [prop](const std::pair<unsigned int, bool>& o){ return o.first == prop; });

          // Either the prop or its negated version must be a literal
          assert(idx != units.end());
          assert(unit_a[prop] == PropAssignment::Unassigned);

          if ((*idx).second) {
            unit_a[prop] = PropAssignment::False; 
          } else {
            unit_a[prop] = PropAssignment::True;
          } 

          assert(unit_a != a);
          
          Simplification unit_s = formula_.simplify(unit_a, s);
          frontier_.push({unit_a, unit_s});
        }

        // The Assignment portion of the return value will be empty unless the
        // DPLLResult part is "Satisfiable"
        std::pair<DPLLResult, Assignment> iterate() {
          auto current = frontier_.top();
          frontier_.pop();

          Assignment a = current.first;
          Simplification s = current.second;

          Assignment empty;
          auto e = formula_.eval(a, s);
          if (e == PropAssignment::True) {
            return {DPLLResult::Satisfiable, a}; 
          } else if (e == PropAssignment::False) {
            return {DPLLResult::Unsatisfiable, empty};
          }

          // Only do splitting rule if there are no unit clauses
          if (formula_.get_unit_clause_props(a, s).size() == 0) {
            do_splitting_rule(a, s);
          }
          
          // TODO Add pure literals
          // do_pure_literals(a, s);

          // Add unit clauses
          do_unit_preference(a, s);
           
          return {DPLLResult::Unknown, empty};
        }

        friend std::tuple<DPLLResult, Assignment, int> dpll<F, G>(CNFFormula f,
            F ph_func, G ah_func, const std::chrono::seconds cutoff);

      private:
        CNFFormula formula_;
        std::stack<std::pair<Assignment, Simplification>> frontier_;
        PropositionHeuristic<F> ph_;
        AssignmentHeuristic<G> ah_; 

    };

    // Free Functions
    
    // ph_func should return a vector of ranks with the same length as its
    // input vector, and ah_func should return true or false for assignments.
    template <typename F, typename G>
    std::tuple<DPLLResult, Assignment, int> dpll(CNFFormula f,
        F ph_func, G ah_func, const std::chrono::seconds cutoff) {

      if (f.get_num_props() == 0) {
        std::valarray<PropAssignment> empty = {};
        return {DPLLResult::Satisfiable, empty, 0};
      }

      DPLLState<F, G> state(f, PropositionHeuristic<F>(ph_func),
          AssignmentHeuristic<G>(ah_func));

      int calls = 0;
      auto start_time = std::chrono::steady_clock::now();
      while (true) {
        DPLLResult r;
        Assignment a;
        calls += 1;

        std::tie(r, a) = state.iterate();
        if (r == DPLLResult::Satisfiable) {
          return std::make_tuple(r, a, calls);
        } else if (r == DPLLResult::Unsatisfiable && state.frontier_.size() == 0) {
          log_info("UNSAT");
          return std::make_tuple(r, a, calls);
        }
        // TODO
        // else if (r == DPLLResult::Unsatisfiable) {
        //   // TODO Do CDCL: Add conflicting clause to formula, call dpll recursively
        //   CNFFormula f_conflict = f;
        //   Clause conflict_clause = f_conflict.get_conflict_clause(a));
        //   log_info("Adding conflicting clause", conflict_clause, "and recursing");
        //   f_conflict.add_clause(conflict_clause);
        //
        //   int c;
        //   std::tie(r, a, c) = dpll(f_conflict, ph_func, ah_func, cutoff);
        //   return std::make_tuple(r, a, c + calls);
        // }

        auto current_time = std::chrono::steady_clock::now();
        auto time_taken =
          std::chrono::duration_cast<std::chrono::seconds>(current_time -
              start_time);
        if (time_taken > cutoff) {
          log_info("Cutoff time of", cutoff.count(), "seconds exceeded");
          return std::make_tuple(r, a, calls);   
        }

        //log_info("Iterating DPLL with", state.frontier_.size(), "assignments in stack");
      }
    }

    std::ostream& operator<<(std::ostream& os, const DPLLResult& r);


  } // namespace logic
} // namespace cannon

#endif /* ifndef CANNON_LOGIC_DPLL_H */
