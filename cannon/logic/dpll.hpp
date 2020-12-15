#ifndef CANNON_LOGIC_DPLL_H
#define CANNON_LOGIC_DPLL_H 

#include <stack>
#include <utility>
#include <cassert>
#include <algorithm>
#include <tuple>
#include <chrono>
#include <map>
#include <queue>
#include <functional>
#include <iostream>
#include <fstream>
#include <memory>

#include <cannon/logic/cnf.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::log;

namespace cannon {
  namespace logic {

    using Assignment=std::valarray<PropAssignment>;
    using Simplification=std::valarray<bool>;
    using AssignmentParents=std::valarray<int>;
    using AssignmentLevels=std::valarray<int>;

    using PropFunc = std::function<std::vector<double>(const CNFFormula&, 
        const Assignment&, 
        const Simplification&, 
        const std::vector<unsigned int>&, 
        const std::vector<std::vector<unsigned int>>&,
        const VectorXd& 
        )>;

    using AssignFunc = std::function<bool(const CNFFormula&, 
        const Assignment&, 
        const Simplification&, 
        unsigned int,
        const std::vector<std::vector<unsigned int>>&
        )>;

    std::vector<double> default_prop(const CNFFormula& form, const Assignment&
        a,const Simplification& s, const std::vector<unsigned int>& props,
        const std::vector<std::vector<unsigned int>>& watched, const VectorXd& vsids);

    bool default_assign(const CNFFormula& form, const
        Assignment& a, const Simplification& s, unsigned int prop, const
        std::vector<std::vector<unsigned int>>& watched);

    class FormulaState {
      public:
        FormulaState(const Assignment &a, const Simplification& s, const
            AssignmentParents& parents, const AssignmentLevels& levels, const
            std::vector<bool>& level_open, int decision_level, int decision_prop)
          : a(a), s(s), parents(parents), levels(levels), level_open(level_open),
          decision_level(decision_level), decision_prop(decision_prop) {
            for (unsigned int i = 0; i < a.size(); i++) {
              if (a[i] == PropAssignment::Unassigned) {
                untried_props.push_back(i);
              }
            } 
          }

        FormulaState(const FormulaState& fs) : a(fs.a), s(fs.s),
        parents(fs.parents), levels(fs.levels), level_open(fs.level_open),
        untried_props(fs.untried_props),
        decision_level(fs.decision_level), decision_prop(fs.decision_prop) {}

        FormulaState(FormulaState&& fs) : a(std::move(fs.a)), s(std::move(fs.s)),
        parents(std::move(fs.parents)), levels(std::move(fs.levels)), level_open(std::move(fs.level_open)),
        untried_props(std::move(fs.untried_props)),
        decision_level(fs.decision_level), decision_prop(fs.decision_prop) {}

        FormulaState& operator=(const FormulaState &fs) {
          a = fs.a;
          s = fs.s;
          parents = fs.parents;
          levels = fs.levels;
          level_open = fs.level_open;
          untried_props = fs.untried_props;
          decision_level = fs.decision_level;
          decision_prop = fs.decision_prop;

          return *this;
        }

        // TODO Can write to a temp file and wait for user input to continue.
        // Also, only write graph when a clause is learned, as that's the
        // inflection point we care about.
        void write_graph() {
          std::ofstream os("tmp.dot");
          os << "digraph dpll_state {" << std::endl;

          // TODO Write graph (check assignment, level_open, etc.)
          // Arrow back from conflict level to branch where backjumping will restore

          os << "}" << std::endl;

          do {
            std::cout << "Press Enter to continue..." << std::endl;
          } while (std::cin.get() != '\n');

          return;
        }

        Assignment a;
        Simplification s;
        AssignmentParents parents;
        AssignmentLevels levels;
        std::vector<bool> level_open;

        std::vector<unsigned int> untried_props;

        int decision_level;
        int decision_prop;
    };

    enum class DPLLResult {
      Satisfiable,
      Unknown,
      Unsatisfiable
    };

    // Heuristic functor for use when choosing propositions
    class PropositionHeuristic {
      public:

        PropositionHeuristic() = delete;

        PropositionHeuristic(PropFunc f) : f_(f) {}

        std::vector<double> rank_props(const CNFFormula& form, const
            Assignment& a, const Simplification& s, const std::vector<unsigned
            int>& props, const std::vector<std::vector<unsigned int>>& watched,
            const VectorXd& vsids) {
          return f_(form, a, s, props, watched, vsids);
        }

        unsigned int choose_prop(const CNFFormula& form, const Assignment& a,
            const Simplification& s, const std::vector<unsigned int>& props,
            const std::vector<std::vector<unsigned int>>& watched, const VectorXd& vsids) {
          auto prop_ranks = rank_props(form, a, s, props, watched, vsids);

          unsigned int prop_idx = std::distance(prop_ranks.begin(),
              std::max_element(prop_ranks.begin(), prop_ranks.end()));

          return props[prop_idx];
        }

      private:
        PropFunc f_;

    };

    // Heuristic functor for use when applying splitting rule
    class AssignmentHeuristic {
      public:
        AssignmentHeuristic() = delete;

        AssignmentHeuristic(AssignFunc f) : f_(f) {}

        bool choose_assignment(const CNFFormula& form, const Assignment& a, const
            Simplification& s, unsigned int prop, const std::vector<std::vector<unsigned int>>& watched) {
          return f_(form, a, s, prop, watched);
        }

      private:
        AssignFunc f_;
      
    };


    class DPLLState {
      public:

        DPLLState() = delete; 
        
        DPLLState(CNFFormula f, PropositionHeuristic ph,
            AssignmentHeuristic ah) : formula_(f), ph_(ph), ah_(ah) {
          vsids_ = VectorXd::Zero(formula_.get_num_props());

          num_original_clauses_ = formula_.clauses_.size(); 
          max_original_clause_size_ = 0;
          for (auto &c : formula_.clauses_) {
            if (c.literals_.size() > max_original_clause_size_)
              max_original_clause_size_ = c.literals_.size();

            for (auto &l : c.literals_) {
              vsids_[l.prop_] += 1.0;
            }
          }
          do_preprocessing();
        }

        Simplification check_simplification_size(const Simplification& s);
        Assignment do_pure_literal_deletion(const Assignment& a_0, const Simplification& s_0);
        void check_watched(std::shared_ptr<FormulaState> fs);

        void restart();
        void do_preprocessing();

        void do_all_resolutions(std::shared_ptr<FormulaState> fs, Clause& c);
        int compute_lbd(std::shared_ptr<FormulaState> fs, const Clause& c);
        int learn_clause(std::shared_ptr<FormulaState> fs, unsigned int c, unsigned int prop);

        bool update_watch_and_propagate(std::shared_ptr<FormulaState> fs,
            std::set<std::pair<unsigned int, unsigned int>> unit_clauses);

        std::pair<std::set<std::pair<unsigned int, unsigned int>>,
          std::pair<int, int>> update_watched(std::shared_ptr<FormulaState> fs, unsigned int prop);

        void do_unit_preference(std::shared_ptr<FormulaState> fs, unsigned int unit_clause);

        bool do_splitting_rule(const std::shared_ptr<FormulaState> fs);

        void do_backjump();
        // The Assignment portion of the return value will be empty unless the
        // DPLLResult part is "Satisfiable"
        std::pair<DPLLResult, Assignment> iterate();

        friend std::tuple<DPLLResult, Assignment, int> dpll(CNFFormula f,
            PropFunc ph_func, AssignFunc ah_func, const std::chrono::seconds cutoff);

      private:
        CNFFormula formula_;
        std::stack<std::shared_ptr<FormulaState>> frontier_;
        PropositionHeuristic ph_;
        AssignmentHeuristic ah_; 
        std::vector<std::vector<unsigned int>> watched;
        std::vector<std::map<unsigned int, bool>> clause_watched;

        VectorXd vsids_;
        double vsids_decay_ = 0.99;
        double lbd_ema_ = 1.0;
        const double lbd_ema_decay_ = 0.95;
        
        
        int iterations_ = 0;
        int restart_iterations_ = 1000;
        int total_iterations = 0;

        std::vector<int> learned_usage;

        unsigned int max_original_clause_size_;
        unsigned int num_original_clauses_;

        bool found_unsat_ = false;
        bool should_restart_ = false;

        int backjump_ = -1;
        bool should_backjump_ = false;

        bool learned_clause_ = false;
    };

    // Free Functions
    
    // ph_func should return a vector of ranks with the same length as its
    // input vector, and ah_func should return true or false for assignments.
    std::tuple<DPLLResult, Assignment, int> dpll(CNFFormula f, PropFunc
        ph_func, AssignFunc ah_func, const std::chrono::seconds
        cutoff=std::chrono::seconds(1200));

    // Convenience
    std::tuple<DPLLResult, Assignment, int> dpll(CNFFormula f, const
        std::chrono::seconds cutoff=std::chrono::seconds(1200));

    std::ostream& operator<<(std::ostream& os, const DPLLResult& r);


  } // namespace logic
} // namespace cannon

#endif /* ifndef CANNON_LOGIC_DPLL_H */
