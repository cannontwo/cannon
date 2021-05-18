#ifndef CANNON_LOGIC_DPLL_H
#define CANNON_LOGIC_DPLL_H 

#include <stack>
#include <utility>
#include <algorithm>
#include <tuple>
#include <chrono>
#include <map>
#include <functional>
#include <iostream>
#include <fstream>
#include <memory>
#include <random>

#include <cannon/logic/cnf.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::log;

namespace cannon {
  namespace logic {

    class DPLLState;
    class FormulaState;

    using Assignment=std::valarray<PropAssignment>;
    using Simplification=std::valarray<bool>;
    using AssignmentParents=std::valarray<int>;
    using AssignmentLevels=std::valarray<int>;

    using PropFunc = std::function<unsigned int(const DPLLState& ds,
        const Assignment&, const Simplification&, std::vector<unsigned int>&)>;

    using AssignFunc = std::function<bool(const CNFFormula&, 
        const Assignment&, 
        const Simplification&, 
        unsigned int,
        const std::vector<std::vector<unsigned int>>&
        )>;

    unsigned int default_prop(const DPLLState& ds, const Assignment& a,
        const Simplification& s, std::vector<unsigned int>& props);

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

        unsigned int choose_prop(const DPLLState& ds,
            const Assignment& a, const Simplification& s, std::vector<unsigned int>& props) {
          return f_(ds, a, s, props);
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

    using Comparator = std::function<bool(const unsigned int&, const unsigned int&)>;

    struct FalseComparator {
      bool operator()(const unsigned int& idx1, const unsigned int& idx2) {
        return false;
      }
    };

    struct RandomComparator {
      RandomComparator(const DPLLState &ds) {
        std::random_device rd;
        std::default_random_engine gen(rd());
        std::uniform_real_distribution<double> d(0.0, 1.0);

        for (unsigned int i = 0; i < ds.formula_.get_num_props(); i++) {
          random_vec.push_back(d(gen));
        }
      }

      RandomComparator(const RandomComparator& rc) : random_vec(rc.random_vec) {}

      RandomComparator(RandomComparator&& rc) : random_vec(std::move(rc.random_vec)) {}

      bool operator()(const unsigned int& idx1, const unsigned int& idx2) {
        return random_vec[idx1] < random_vec[idx2];
      }

      std::vector<unsigned int> random_vec;

    };

    struct TwoClauseComparator {
      TwoClauseComparator(const DPLLState &ds, const Assignment& a, const Simplification& s,
          const std::vector<unsigned int>& props, Comparator next) : next(next) {

        for (unsigned int i = 0; i < ds.formula_.get_num_props(); i++) {
          clause_num_vec.push_back(0);
        }

        std::vector<unsigned int> tmp_clause_num_vec = ds.formula_.get_num_two_clauses(a, s, props);
        for (unsigned int i = 0; i < tmp_clause_num_vec.size(); i++) {
          clause_num_vec[props[i]] = tmp_clause_num_vec[i];
        }

        max_two_clauses = *std::max_element(clause_num_vec.begin(), clause_num_vec.end());
      }

      TwoClauseComparator(const TwoClauseComparator& tc) : next(tc.next),
      clause_num_vec(tc.clause_num_vec), max_two_clauses(tc.max_two_clauses) {}

      TwoClauseComparator(TwoClauseComparator&& tc) : next(std::move(tc.next)),
      clause_num_vec(std::move(tc.clause_num_vec)), max_two_clauses(tc.max_two_clauses) {}

      bool operator()(const unsigned int& idx1, const unsigned int& idx2) {
        if (clause_num_vec[idx1] == clause_num_vec[idx2]) {
          return next(idx1, idx2);
        } else {
          return clause_num_vec[idx1] < clause_num_vec[idx2];
        }
      }

      Comparator next;
      std::vector<unsigned int> clause_num_vec;
      unsigned int max_two_clauses;
    };

    struct VsidsComparator {
      VsidsComparator(const DPLLState &ds, Comparator next) : next(next), vsids(ds.vsids_) {}
      
      VsidsComparator(const VsidsComparator& vc) : next(vc.next), vsids(vc.vsids) {}

      VsidsComparator(VsidsComparator&& vc) : next(std::move(vc.next)), vsids(std::move(vc.vsids)) {}

      bool operator()(const unsigned int& idx1, const unsigned int& idx2) {
        if (vsids[idx1] == vsids[idx2]) 
          return next(idx1, idx2);
        else
          return vsids[idx1] < vsids[idx2];
      }

      Comparator next;
      VectorXd vsids;

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
