#ifndef CANNON_LOGIC_DPLL_H
#define CANNON_LOGIC_DPLL_H 

/*!
 * \file cannon/logic/dpll.hpp
 * \brief File containing classes and logic for running the DPLL and CDCL
 * algorithms for finding satisfying assignments for CNF formulas.
 *
 * See https://en.wikipedia.org/wiki/DPLL_algorithm and
 * https://en.wikipedia.org/wiki/Conflict-driven_clause_learning
 */

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

    /*!
     * \brief Default proposition sorting heuristic. Used to sort propoositions
     * for exploration in the DPLL algorithm.
     *
     * \param ds Current state of DPLL algorithm.
     * \param a Current assignment 
     * \param s Current simplification
     * \param props Propositions to sort using default heuristics
     *
     * \returns Proposition chosen by this heuristic.
     */
    unsigned int default_prop(const DPLLState& ds, const Assignment& a,
        const Simplification& s, std::vector<unsigned int>& props);

    /*!
     * \brief Default assignment choice heuristic. Used to select which boolean
     * assignment to explore first for an input proposition.
     *
     * \param form The formula under consideration.
     * \param a The current assignment.
     * \param s The current simplification.
     * \param prop The proposition to choose an assignment for.
     * \param watched The watched list for speeding up formula analysis.
     *
     * \returns Assignment chosen by heuristic.
     */
    bool default_assign(const CNFFormula& form, const
        Assignment& a, const Simplification& s, unsigned int prop, const
        std::vector<std::vector<unsigned int>>& watched);

    /*!
     * \brief Class representing the state of a formula on the frontier
     * explored by the DPLL algorithm, which includes a partial assignment and
     * associated formula simplification. This class also keeps information to
     * assist backtracking and clause learning.
     */
    class FormulaState {
      public:

        /*!
         * \brief Constructor.
         *
         * \param a Assignment for this state
         * \param s Simplification for this state
         * \param parents Parent clause causing assignment for each proposition
         * \param levels Level of search when each proposition was assigned
         * \param level_open Levels of search still open for search
         * \param decision_level Current level of search
         * \param decision_prop Proposition split on at the current level
         */
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

        /*!
         * \brief Copy constructor.
         */
        FormulaState(const FormulaState& fs) : a(fs.a), s(fs.s),
        parents(fs.parents), levels(fs.levels), level_open(fs.level_open),
        untried_props(fs.untried_props),
        decision_level(fs.decision_level), decision_prop(fs.decision_prop) {}

        /*!
         * \brief Move constructor.
         */
        FormulaState(FormulaState&& fs) : a(std::move(fs.a)), s(std::move(fs.s)),
        parents(std::move(fs.parents)), levels(std::move(fs.levels)), level_open(std::move(fs.level_open)),
        untried_props(std::move(fs.untried_props)),
        decision_level(fs.decision_level), decision_prop(fs.decision_prop) {}

        /*!
         * \brief Copy assignment operator.
         */
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

        /*!
         * \brief Write the current DPLL search tree to a DOT file.
         */
        void write_graph();

      public:
        Assignment a; //!< Assignment for this state in the search
        Simplification s; //!< Simplification for this state in the search
        AssignmentParents parents; //!< Parent clause causing assignment for each proposition, -1 if unassigned, -2 if split on during search
        AssignmentLevels levels; //!< Level of search when each proposition was assigned
        std::vector<bool> level_open; //!< Whether each level of search is still open for search

        std::vector<unsigned int> untried_props; //!< Propositions which have not been split on at this level

        int decision_level; //!< Decision level of this state
        int decision_prop; //!< Proposition split on at this level
    };

    /*!
     * \brief Struct representing the possible termination states of the DPLL algorithm.
     */
    enum class DPLLResult {
      Satisfiable,
      Unknown,
      Unsatisfiable
    };

    /*!
     * \brief Functor representing a proposition choice heuristic.
     */
    class PropositionHeuristic {
      public:

        PropositionHeuristic() = delete;

        /*!
         * \brief Constructor taking a ranking function.
         */
        PropositionHeuristic(PropFunc f) : f_(f) {}

        /*!
         * \brief Choose proposition, heuristically.
         *
         * \param ds Current state of DPLL algorithm.
         * \param a Current assignment 
         * \param s Current simplification
         * \param props Propositions to sort using default heuristics
         *
         * \returns Chosen proposition.
         */
        unsigned int choose_prop(const DPLLState& ds,
            const Assignment& a, const Simplification& s, std::vector<unsigned int>& props) {
          return f_(ds, a, s, props);
        }

      private:
        PropFunc f_; //!< Proposition ranking heuristic.

    };

    /*!
     * \brief Functor representing an assignment choice heuristic.
     */
    class AssignmentHeuristic {
      public:
        AssignmentHeuristic() = delete;

        /*!
         * \brief Constructor taking a choice function.
         */
        AssignmentHeuristic(AssignFunc f) : f_(f) {}

        /*!
         * \brief Choose assignment for proposition, heuristically.
         *
         * \param form The formula under consideration.
         * \param a The current assignment.
         * \param s The current simplification.
         * \param prop The proposition to choose an assignment for.
         * \param watched The watched list for speeding up formula analysis.
         *
         * \returns Chosen assignment.
         */
        bool choose_assignment(const CNFFormula& form, const Assignment& a, const
            Simplification& s, unsigned int prop, const std::vector<std::vector<unsigned int>>& watched) {
          return f_(form, a, s, prop, watched);
        }

      private:
        AssignFunc f_; //!< Assignment ranking heuristic.
      
    };

    /*!
     * \brief Class representing the state of the DPLL/CDCL algorithm. Manages
     * a frontier of formula states in order to perform search for satisfying
     * assignments.
     */
    class DPLLState {
      public:

        DPLLState() = delete; 
        
        /*!
         * \brief Constructor taking a formula to check for satisfiability,
         * proposition heuristic, and assignment heuristic.
         */
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

        /*!
         * \brief Check simplification size and, if the simplification is too
         * short, extend it. This behavior is necessary because the formula may
         * grow as we do clause learning.
         *
         * \param s Simplification to check.
         *
         * \returns A new simplification whose size is correct.
         */
        Simplification check_simplification_size(const Simplification& s);
        
        /*!
         * \brief Find pure literals in the current formula and "delete" them
         * by assigning them satisfying values.
         *
         * \param a_0 Current assignment
         * \param s_0 Current simplification
         *
         * \returns A new assignment which deals with pure literals.
         */
        Assignment do_pure_literal_deletion(const Assignment& a_0, const Simplification& s_0);

        /*!
         * \brief Restart DPLL search. Primarily called when a unit clause is
         * learned, as this can dramatically affect the search.
         */
        void restart();

        /*!
         * \brief Preprocess formula by deleting pure literals, simplifying as
         * much as possible, and resolving unit clauses.
         */
        void do_preprocessing();

        /*!
         * \brief Do all possible resolutions for the input clause and input formula state.
         *
         * \param fs The state of the formula to do resolutions on.
         * \param c The clause to resolve with every other. Will be modified as resolutions occur.
         */
        void do_all_resolutions(std::shared_ptr<FormulaState> fs, Clause& c);

        /*!
         * \brief Compute the Literal Block Distance (number of distinct
         * decision levels) of the input clause.
         *
         * \param fs The current formula state.
         * \param c The clause to compute LBD for.
         *
         * \returns LBD for the input clause.
         */ 
        int compute_lbd(std::shared_ptr<FormulaState> fs, const Clause& c);

        /*!
         * \brief Learn a new clause for the formula being searched over by
         * performing all possible resolutions, then add it to the formula.
         *
         * \param fs The current formula state.
         * \param c Index of the existing clause to use for learning.
         * \param prop Proposition that led to conflict triggering learning.
         *
         * \returns Level of decision tree to backjump to, or -1 if something went wrong.
         */
        int learn_clause(std::shared_ptr<FormulaState> fs, unsigned int c, unsigned int prop);

        /*!
         * \brief Propagate assignments implied by the input unit clauses,
         * then update the watched list and handle any new conflicts or unit
         * clauses. The cascade of changes to the formula done by this method
         * can save a lot of unnecessary recursion.
         *
         * \param fs The current formula state.
         * \param unit_clauses Unit clauses to use for initial propagation.
         *
         * \returns Whether a conflict was found during propagation.
         */
        bool update_watch_and_propagate(std::shared_ptr<FormulaState> fs,
            std::set<std::pair<unsigned int, unsigned int>> unit_clauses);

        /*!
         * \brief Update watched list, given that the input proposition has
         * been assigned. The assignment of a previously unassigned proposition
         * watched for a particular clause triggers watching of a new
         * proposition in that clause. Updating the watched list also lets us
         * identify unit clauses and conflicts.
         *
         * \param fs The current formula state
         * \param prop The proposition which was recently assigned.
         *
         * \returns A pair of sets of indices. The first set is the discovered
         * unit clauses, and the second set is the discovered conflicts.
         */
        std::pair<std::set<std::pair<unsigned int, unsigned int>>,
          std::pair<int, int>> update_watched(std::shared_ptr<FormulaState> fs, unsigned int prop);

        /*!
         * \brief Simplify the input formula state by performing the "unit
         * preference" rule of DPLL, i.e., setting the proposition in the input
         * unit clause to the satisfying value implied by the unit clause.
         *
         * \param fs The formula state to apply unit preference on.
         * \param unit_clause Index of unit clause.
         */
        void do_unit_preference(std::shared_ptr<FormulaState> fs, unsigned int unit_clause);

        /*!
         * \brief Perform the "splitting rule" part of DPLL, i.e., choose a
         * proposition heuristically and formula states with both possible
         * assignments to the DPLL frontier.
         *
         * \param fs The formula state to split from.
         *
         * \returns Whether splitting was possible.
         */
        bool do_splitting_rule(const std::shared_ptr<FormulaState> fs);

        /*!
         * \brief Unroll the state of DPLL search to the last open decision level.
         */
        void do_backjump();

        /*!
         * \brief Perform an iteration of the DPLL algorithm by popping a
         * formula state from the frontier and processing it.
         *
         * The Assignment portion of the return value will be empty unless the
         * DPLLResult part is "Satisfiable"
         *
         * \returns Unsatisfiable if the input formula was provably
         * unsatisfiable, Satisfiable if a satisfying assignment was found, and
         * Unknown if more iterations are needed.
         */
        std::pair<DPLLResult, Assignment> iterate();

        friend std::tuple<DPLLResult, Assignment, int> dpll(CNFFormula f,
            PropFunc ph_func, AssignFunc ah_func, const std::chrono::seconds cutoff);

      public:

        CNFFormula formula_; //!< Base formula whose satisfiability is to be evaluated
        std::stack<std::shared_ptr<FormulaState>> frontier_; //!< DPLL search frontier
        PropositionHeuristic ph_; //!< Proposition choice heuristic
        AssignmentHeuristic ah_;  //!< Assignment choice heuristic
        std::vector<std::vector<unsigned int>> watched; //!< Watched list 
        std::vector<std::map<unsigned int, bool>> clause_watched; //!< Inverse watched list

        VectorXd vsids_; //!< VSIDS weights for proposition choice heuristic
        double vsids_decay_ = 0.99; //!< VSIDS decay parameter
        double lbd_ema_ = 1.0; //!< LBD threshold
        const double lbd_ema_decay_ = 0.95; //!< LBD threshold decay parameter
        
        int iterations_ = 0; //!< Number of iterations done since last restart
        int restart_iterations_ = 1000; //!< Iteration restart threshold (can speed up solution)
        int total_iterations = 0; //!< Total number of iterations

        std::vector<int> learned_usage; //!< Usage of each proposition in learned clauses (for heuristics)

        unsigned int max_original_clause_size_; //!< Maximum clause size in original formula
        unsigned int num_original_clauses_; //!< Number of clauses in original formula

        bool found_unsat_ = false; //!< Whether the formula has been found to be unsatisfiable
        bool should_restart_ = false; //!< Whether the DPLL algorithm should restart

        int backjump_ = -1; //!< Backjump level, i.e., last open decision level
        bool should_backjump_ = false; //!< Whether the DPLL algorithm should backjump

        bool learned_clause_ = false; //!< Whether a clause has been learned
    };

    using Comparator = std::function<bool(const unsigned int&, const unsigned int&)>;

    /*!
     * \brief Struct representing a comparison function that always returns false.
     */
    struct FalseComparator {
      bool operator()(const unsigned int& idx1, const unsigned int& idx2) {
        return false;
      }
    };

    /*!
     * \brief Struct representing a comparison function that returns a random
     * comparison result for all indices in a set. This comparison is
     * consistent, as the random choices are cached.
     */
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

    /*!
     * \brief Struct representing a comparison function that considers one
     * proposition greater than another if it participates in a greater number
     * of two-clauses in a formula.
     */
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

    /*!
     * \brief Struct representing a comparison function that considers one
     * proposition greater than another if it has a greater VSIDS value.
     */
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
    
    /*!
     * \brief Function to run the entire DPLL/CDCL algorithm, iterating until a
     * non-Unknown result is returned or a cutoff execution time is reached.
     *
     * \param f The formula to check for satisfiability
     * \param ph_func Proposition choice heuristic
     * \param ah_func Assignment choice heuristic
     * \param cutoff Maximum amount of time that the algorithm should run
     *
     * \returns The result of the DPLL algorithm.
     */
    std::tuple<DPLLResult, Assignment, int> dpll(CNFFormula f, PropFunc
        ph_func, AssignFunc ah_func, const std::chrono::seconds
        cutoff=std::chrono::seconds(1200));

    /*!
     * \brief Function to run the entire DPLL/CDCL algorithm, iterating until a
     * non-Unknown result is returned or a cutoff execution time is reached.
     * Passes the default proposition and assignment heuristics.
     *
     * \param f The formula to check for satisfiability
     * \param cutoff Maximum amount of time that the algorithm should run
     *
     * \returns The result of the DPLL algorithm.
     */
    std::tuple<DPLLResult, Assignment, int> dpll(CNFFormula f, const
        std::chrono::seconds cutoff=std::chrono::seconds(1200));

    std::ostream& operator<<(std::ostream& os, const DPLLResult& r);


  } // namespace logic
} // namespace cannon

#endif /* ifndef CANNON_LOGIC_DPLL_H */
