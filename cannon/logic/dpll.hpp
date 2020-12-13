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

    struct FormulaState {
      Assignment a;
      Simplification s;
      AssignmentParents parents;
      AssignmentLevels levels;
      std::vector<std::vector<unsigned int>> watched;
      std::vector<bool> level_open;

      int decision_level;
      int decision_prop;
    };

    enum class DPLLResult {
      Satisfiable,
      Unknown,
      Unsatisfiable
    };

    // Forward declarations
    class DPLLState;

    std::tuple<DPLLResult, Assignment, int> dpll(CNFFormula f,
        PropFunc ph_func, AssignFunc ah_func, const std::chrono::seconds cutoff=std::chrono::seconds(1200));

    // Heuristic functor for use when choosing propositions
    class PropositionHeuristic {
      public:

        PropositionHeuristic() = delete;

        PropositionHeuristic(PropFunc f) : f_(f) {}

        std::vector<double> rank_props(const CNFFormula& form, const Assignment& a,
            const Simplification& s, const std::vector<unsigned int>& props, std::vector<std::vector<unsigned int>> watched, const VectorXd& vsids) {
          return f_(form, a, s, props, watched, vsids);
        }

        unsigned int choose_prop(const CNFFormula& form, const Assignment& a,
            const Simplification& s, const std::vector<unsigned int>& props,
            std::vector<std::vector<unsigned int>> watched, const VectorXd& vsids) {
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

        void restart() {

          while (!frontier_.empty()) {
            frontier_.pop();
          }

          while (!trail_.empty()) {
            trail_.pop();
          }

          // Inject some randomness
          //vsids_ += VectorXd::Random(formula_.get_num_props());
          log_info("Restarting with", formula_.get_num_clauses(), "clauses");
          do_preprocessing();
        }

        Simplification check_simplification_size(const Simplification& s) {
          assert(s.size() <= formula_.get_num_clauses());

          if (s.size() == formula_.get_num_clauses())
            return s;

          Simplification ret_s(false, formula_.get_num_clauses());
          ret_s[std::slice(0, s.size(), 1)] = s;

          return ret_s;
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
          AssignmentParents parents_0 = AssignmentParents(-1, formula_.get_num_props());
          AssignmentLevels levels_0 = AssignmentLevels(-1, formula_.get_num_props());
          Simplification s_0 = Simplification(false, formula_.get_num_clauses());

          Assignment pld_a = do_pure_literal_deletion(a_0, s_0);
          Simplification s = formula_.simplify(pld_a, s_0);

          std::vector<std::vector<unsigned int>> w;
          for (unsigned int i = 0; i < formula_.get_num_props(); i++) {
            w.emplace_back();
          }

          std::vector<bool> level_open;
          level_open.push_back(false);

          bool have_unit_clause = false;
          std::set<std::pair<unsigned int, unsigned int>> unit_props;

          for (unsigned int i = 0; i < formula_.clauses_.size(); i++) {
            if (s[i])
              continue;

            int num_watched = 0;
            unsigned int watch_prop;
            for (auto &l : formula_.clauses_[i].literals_) {
              if (num_watched == 2)
                break;

              if ((!l.negated_ && pld_a[l.prop_] != PropAssignment::False) ||
                  (l.negated_ && pld_a[l.prop_] != PropAssignment::True)) {
                w[l.prop_].push_back(i);
                num_watched += 1;
                watch_prop = l.prop_;
              }
            }

            if (num_watched == 1) {
              // We have a unit clause
              if (verbose_)
                log_info("In preprocessing, unit clause", formula_.clauses_[i]);

              if (formula_.clauses_[i].size(pld_a) == 1) {
                have_unit_clause = true;
                unit_props.insert(std::make_pair(i, watch_prop));
              }
            }

            if (num_watched == 0) {
              if (verbose_)
                log_info("In preprocessing, conflict in", i);
            }

            // We ignore the unsatisfiable case, as that will be resolved in the first iteration of DPLL
          }

          FormulaState tmp_fs = {pld_a, check_simplification_size(s),
            parents_0, levels_0, w, level_open, 0, -1};
          if (have_unit_clause) {
            //while (unit_clauses.size() != 0) {
            //  bool should_do_unit_preference = true;
            //  unsigned int unit_prop, unit_clause;
            //  std::tie(unit_clause, unit_prop) = *unit_clauses.begin();
            //  unit_clauses.erase(unit_clauses.begin());

            //  if (verbose_)
            //    log_info("do_preprocessing revealed unit clause",
            //        formula_.clauses_[unit_clause], "with",
            //        formula_.clauses_[unit_clause].size(tmp_fs.a), "and is_unit",
            //        formula_.clauses_[unit_clause].is_unit(tmp_fs.a));
            //  

            //  if (!formula_.clauses_[unit_clause].is_unit(tmp_fs.a)) {
            //    assert(formula_.clauses_[unit_clause].eval(tmp_fs.a) == PropAssignment::True);
            //    log_info("Skipping because clause", formula_.clauses_[unit_clause], "resolved already as", formula_.clauses_[unit_clause].eval(tmp_fs.a));

            //    should_do_unit_preference = false;
            //  }
            //  
            //  if (should_do_unit_preference)
            //    do_unit_preference(tmp_fs, unit_clause);

            //  std::set<std::pair<unsigned int, unsigned int>> tmp_unit_clauses;
            //  std::pair<int, int> tmp_conflict_clause;
            //  std::tie(tmp_unit_clauses, tmp_conflict_clause) =
            //    update_watched(tmp_fs, unit_prop);

            //  if (tmp_conflict_clause.first >= 0) {
            //    learn_clause(tmp_fs, tmp_conflict_clause.first, tmp_conflict_clause.second);
            //    should_restart_ = true;
            //    break;
            //  } else {
            //    unit_clauses.insert(tmp_unit_clauses.begin(), tmp_unit_clauses.end());
            //  }
            //}
            
            bool found_conflict = update_watch_and_propagate(tmp_fs, unit_props);
            if (verbose_ && found_conflict)
              log_info("Found conflict in preprocessing"); 

            frontier_.push(tmp_fs);
          } else
            frontier_.push(tmp_fs); 

          if (verbose_)
            log_info("Finished preprocessing");
        }

        int learn_clause(FormulaState &fs, unsigned int c, unsigned int prop) {
          // We have a conflict, so we can terminate
          if (verbose_) {
            log_info("learn_clause called with", c, prop);
            log_info("Learning clause with conflict clause", formula_.clauses_[c], "and conflict prop", prop);
          }

          assert(formula_.clauses_[c].eval(fs.a) == PropAssignment::False);

          if (fs.parents[prop] == -2) {
            if (verbose_) {
              log_info("Prop", prop, "resulting from splitting",
                  "led to a conflict in clause", 
                  formula_.clauses_[c]);
            }
          } else if (fs.parents[prop] >= 0) {
            if (formula_.clauses_[fs.parents[prop]] == formula_.clauses_[c]) {
              log_info("ALERT");
            }

            if (verbose_) {
              log_info("Prop", prop, "set to", fs.a[prop], "with parent clause",
                  formula_.clauses_[fs.parents[prop]],"led to a conflict in clause", 
                  formula_.clauses_[c]);

              log_info("Conflict clause", formula_.clauses_[c]);
            }

            if (fs.parents[prop] > num_original_clauses_) {
              learned_usage[fs.parents[prop] - num_original_clauses_] += 1;
            }
            if (c > num_original_clauses_) {
              learned_usage[c - num_original_clauses_] += 1;
            }

            // Do all possible resolutions
            Clause learned_c = formula_.clauses_[c];
            Literal resolve_literal = *(learned_c.literals_.begin());
            int num_current_level = 0;
            for (auto &l : learned_c.literals_) {
              if (fs.parents[l.prop_] >= 0 && (fs.levels[l.prop_] >= fs.levels[resolve_literal.prop_]))
                resolve_literal = l;
              
              if (fs.levels[l.prop_] == fs.decision_level)
                num_current_level += 1;
            }

            // Exponential moving average vsids
            vsids_ *= (1.0 - vsids_decay_);

            while (num_current_level > 1 && learned_c.size() > 0) {
              num_current_level = 0;

              if (verbose_) {
                log_info("Resolving", learned_c, "on", resolve_literal, "with parent",
                    formula_.clauses_[fs.parents[resolve_literal.prop_]], "set at level", fs.levels[resolve_literal.prop_]);
              }

              resolve(learned_c,
                  formula_.clauses_[fs.parents[resolve_literal.prop_]],
                  resolve_literal.prop_);

              for (auto &l : formula_.clauses_[fs.parents[resolve_literal.prop_]].literals_) {
                vsids_[l.prop_] += vsids_decay_;
              }

              if (fs.parents[resolve_literal.prop_] > num_original_clauses_) {
                learned_usage[fs.parents[resolve_literal.prop_] - num_original_clauses_] += 1;
              }

              num_current_level = 0;
              resolve_literal = *(learned_c.literals_.begin());
              for (auto &l : learned_c.literals_) {
                if ((fs.parents[l.prop_] >= 0) && (fs.levels[l.prop_] >=
                      fs.levels[resolve_literal.prop_])) {
                  resolve_literal = l;
                }
                
                if (fs.levels[l.prop_] == fs.decision_level) {
                  num_current_level += 1;
                }
              }

              if (num_current_level == 1) {
                break;
              } 
            }

            int prev_clauses = formula_.get_num_clauses();
            formula_.add_clause(std::move(learned_c));
            int post_clauses = formula_.get_num_clauses();

            // TODO Calculate LBD, add to vector

            // Keep track of usage of learned clauses

            if (verbose_) {
              log_info("Current level is", fs.decision_level);
              log_info("Learned clause", learned_c);
              log_info("Clause levels are");
              for (auto &l : learned_c.literals_) {
                log_info("\t", fs.levels[l.prop_], ",", fs.level_open[fs.levels[l.prop_]]);  
              }
            }

            if (learned_c.literals_.size() == 1) {
              log_info("Learned unit clause", learned_c);
              should_restart_ = true;
            }

            if (learned_c.literals_.size() == 0) {
              log_info("Learned empty clause (conflict)", learned_c);
              found_unsat_ = true;
            }

            int num_learned_watched = 0;
            int max_decision_level = -1;
            for (auto &l : learned_c.literals_) {

              // Keep track of latest decision level contributing to this conflict
              //if ((fs.levels[l.prop_] > max_decision_level) &&
              //    fs.level_open[fs.levels[l.prop_]] && (fs.levels[l.prop_] != fs.decision_level)) 
              if ((fs.levels[l.prop_] > max_decision_level) &&
                  fs.level_open[fs.levels[l.prop_]]) 
                max_decision_level = fs.levels[l.prop_];

            }

            if (prev_clauses != post_clauses) {
              learned_usage.push_back(0);
              int new_clause_num = formula_.get_num_clauses();

              for (auto &l : learned_c.literals_) {
                if (num_learned_watched < 2) {
                  if ((!l.negated_ && fs.a[l.prop_] != PropAssignment::False)
                      || (l.negated_ && fs.a[l.prop_] != PropAssignment::True)) {
                    fs.watched[l.prop_].push_back(new_clause_num);
                    num_learned_watched += 1;
                  }
                }
              }
            }

            // Backjump by unwinding the stack of formulas states
            if (verbose_)
              log_info("Should backjump to level", max_decision_level);

            return max_decision_level;

            // TODO Move

              //assert(jumped_decision_level == max_decision_level);
              // ============


              //if (num_jumped > 0) {
                // Swap
                //if (frontier_.top().decision_level == pop_fs.decision_level) {
                //  FormulaState swap_fs = frontier_.top();
                //  frontier_.pop();

                //  if (verbose_)
                //    log_info("Swapping formula states for level", swap_fs.decision_level);

                //  swap_fs.level_open[swap_fs.decision_level] = !swap_fs.level_open[swap_fs.decision_level];
                //  pop_fs.level_open[pop_fs.decision_level] = !pop_fs.level_open[pop_fs.decision_level];

                //  frontier_.push(pop_fs);
                //  frontier_.push(swap_fs);
                //} else {
                //}
              //}

              // TODO Delete?
              // Push restored branch
              //Assignment restored_a = Assignment(PropAssignment::Unassigned, formula_.get_num_props());
              //Simplification restored_s = Simplification(false, formula_.get_num_clauses());
              //AssignmentParents restored_parents = AssignmentParents(-1, formula_.get_num_props());
              //AssignmentLevels restored_levels = AssignmentLevels(-1, formula_.get_num_props());
              //std::vector<std::vector<unsigned int>> restored_w;
              //for (unsigned int i = 0; i < formula_.get_num_props(); i++) {
              //  restored_w.emplace_back();
              //}

              //std::vector<bool> restored_level_open;
              //for (int i = 0; i <= max_decision_level; i++) {
              //  restored_level_open.push_back(fs.level_open[i]);
              //}

              //for (int i = 0; i < formula_.get_num_props(); i++) {
              //  if (fs.levels[i] <= max_decision_level) {
              //    restored_a[i] = fs.a[i];
              //    restored_parents[i] = fs.parents[i];
              //    restored_levels[i] = fs.levels[i];
              //  }
              //}

              //restored_s = formula_.simplify(restored_a, restored_s);

              //for (unsigned int i = 0; i < formula_.clauses_.size(); i++) {
              //  int num_watched = 0;
              //  for (auto &l : formula_.clauses_[i].literals_) {
              //    if (num_watched == 2)
              //      break;

              //    if ((!l.negated_ && restored_a[l.prop_] != PropAssignment::False) ||
              //        (l.negated_ && restored_a[l.prop_] != PropAssignment::True)) {
              //      restored_w[l.prop_].push_back(i);
              //      num_watched += 1;
              //    } 
              //  }

              //  if (num_watched == 0) {
              //    log_info("On restoring, conflict in", c);
              //  }
              //}

              //if (verbose_)
              //  log_info("Restoring branch at level", max_decision_level);

              //FormulaState tmp_fs = {restored_a, check_simplification_size(restored_s),
              //  restored_parents, restored_levels, restored_w,
              //  restored_level_open, max_decision_level};
              //frontier_.push(tmp_fs);

              //if (verbose_) {
              //  if (num_jumped > 0)
              //    log_info("Jumped back", num_jumped, "levels");
              //}
          } else {
            throw std::runtime_error("Conflict arose from unset proposition");
            return -1;
          }

          return -1;
        }

        std::pair<std::set<std::pair<unsigned int, unsigned int>>,
          std::pair<int, int>> update_watched(FormulaState& fs, unsigned int prop) {
          assert(fs.level_open.size() == fs.decision_level + 1);

          std::set<std::pair<unsigned int, unsigned int>> unit_clauses;
          std::pair<int, int> conflict_clause = std::make_pair(-1, -1);

          std::vector<unsigned int> rem_clauses;

          for (unsigned int c : fs.watched[prop]) {
            if (fs.s[c])
              continue;

            bool watch_new = false;

            for (auto &l : formula_.clauses_[c].literals_) {
              if (l.prop_ != prop)
                continue;

              // Past here, l.prop_ == prop

              // If the assignment doesn't evaluate to false here, don't watch anything new
              if ((l.negated_ && (fs.a[prop] == PropAssignment::False)) ||
                  (!l.negated_ && (fs.a[prop] == PropAssignment::True)) || fs.a[prop]
                  == PropAssignment::Unassigned)
                break;

              // Otherwise, we need to watch something new
              rem_clauses.push_back(c);
              watch_new = true;
            }

            if (watch_new) {
              int num_watched = 0;
              unsigned int watch_prop;
              for (auto &l : formula_.clauses_[c].literals_) {
                if (l.prop_ == prop)
                  continue;

                if (num_watched == 2) {
                  break;
                }

                if (std::find(fs.watched[l.prop_].begin(), fs.watched[l.prop_].end(), c) != fs.watched[l.prop_].end()) {
                  // This literal is already watched, so move on
                  num_watched += 1;
                  watch_prop = l.prop_;
                  continue;
                } else {
                  if (fs.a[l.prop_] == PropAssignment::Unassigned || (l.negated_
                        && (fs.a[l.prop_] == PropAssignment::False)) || (!l.negated_
                        && (fs.a[l.prop_] == PropAssignment::True))) {
                    fs.watched[l.prop_].push_back(c);
                    watch_prop = l.prop_;
                    num_watched += 1;
                  }
                }
              }

              if (num_watched == 1) {
                // We may have a unit clause
                if (formula_.clauses_[c].size(fs.a) == 1)
                  unit_clauses.insert(std::make_pair(c, watch_prop));
              }

              if (num_watched == 0) {
                if (verbose_)
                  log_info("Found conflict clause", c, "with prop", prop);

                conflict_clause = std::make_pair(c, prop);
                break;
              }
            }
          }

          for (unsigned int c : rem_clauses) {
            auto end = std::remove(fs.watched[prop].begin(), fs.watched[prop].end(), c);
            fs.watched[prop].erase(end, fs.watched[prop].end());
          }

          return std::make_pair(unit_clauses, conflict_clause);
        }

        void do_splitting_rule(const FormulaState& fs) {
          if (verbose_)
            log_info("Doing splitting rule at decision level", fs.decision_level + 1);


          // Create vector of all propositions
          std::vector<unsigned int> all_props = formula_.get_props(fs.a, fs.s);
          if (all_props.size() == 0) {
            log_info("All props is empty");
            return;
          }

          unsigned int prop = ph_.choose_prop(formula_, fs.a, fs.s, all_props, fs.watched, vsids_);
          bool first_assignment = ah_.choose_assignment(formula_, fs.a, fs.s, prop, fs.watched);

          assert(fs.a[prop] == PropAssignment::Unassigned);
          if (verbose_)
            log_info("Splitting on", prop);
          
          // First branch
          Assignment split_first_a(fs.a);
          split_first_a[prop] = first_assignment ? PropAssignment::True : PropAssignment::False;
          Simplification split_first_s = formula_.simplify(split_first_a, fs.s);

          AssignmentParents split_first_parents(fs.parents);
          AssignmentLevels split_first_levels(fs.levels);

          split_first_parents[prop] = -2;
          split_first_levels[prop] = fs.decision_level + 1;

          std::vector<bool> split_first_level_open(fs.level_open);
          split_first_level_open.push_back(true);

          int tmp_prop = prop;
          FormulaState unsimplified_first_fs = {split_first_a, split_first_s,
            split_first_parents, split_first_levels, fs.watched, split_first_level_open,
            fs.decision_level + 1, tmp_prop};
          //FormulaState unsimplified_first_fs = update_watched(tmp_fs, prop);

          // Second branch
          Assignment split_second_a(fs.a);
          split_second_a[prop] = first_assignment ? PropAssignment::False : PropAssignment::True;
          Simplification split_second_s = formula_.simplify(split_second_a, fs.s);

          AssignmentParents split_second_parents(fs.parents);
          AssignmentLevels split_second_levels(fs.levels);

          split_second_parents[prop] = -2;
          split_second_levels[prop] = fs.decision_level + 1;

          std::vector<bool> split_second_level_open(fs.level_open);
          split_second_level_open.push_back(false);
          FormulaState unsimplified_second_fs = {split_second_a, split_second_s, split_second_parents,
            split_second_levels, fs.watched, split_second_level_open, fs.decision_level
              + 1, tmp_prop};

          //FormulaState unsimplified_second_fs = update_watched(tmp_fs, prop);

          assert(split_first_a != split_second_a);
          assert(split_first_a != fs.a);
          assert(split_second_a != fs.a);

          // Push the second branch to be evaluated first, since we're using a stack
          frontier_.push(unsimplified_second_fs);
          frontier_.push(unsimplified_first_fs);

          if (verbose_)
            log_info("Frontier has size", frontier_.size());
        }

        void do_unit_preference(FormulaState& fs, unsigned int unit_clause) {
          if (verbose_) {
            log_info("Doing unit preference with provided clause");
            log_info(formula_.clauses_[unit_clause]);
          }

          Clause& unit_c = formula_.clauses_[unit_clause];
          assert(unit_c.is_unit(fs.a));

          unsigned int unit_prop = unit_c.get_unit_prop(fs.a);
          unsigned int unit_prop_negated = unit_c.get_unit_negated(fs.a);

          if (verbose_)
            log_info("Doing unit preference on", unit_prop);

          // Either the prop or its negated version must be a literal
          assert(fs.a[unit_prop] == PropAssignment::Unassigned);
          assert(fs.parents[unit_prop] == -1);
          assert(fs.levels[unit_prop] == -1);

          if (unit_prop_negated) {
            fs.a[unit_prop] = PropAssignment::False; 
          } else {
            fs.a[unit_prop] = PropAssignment::True;
          } 
          fs.parents[unit_prop] = unit_clause;
          fs.levels[unit_prop] = fs.decision_level;

          if (verbose_)
            log_info("Prop", unit_prop, "was set to", fs.a[unit_prop], "with parent", formula_.clauses_[fs.parents[unit_prop]]);

          fs.s = formula_.simplify(fs.a, fs.s);
        }

        // TODO Modify to take starting clause, so that only newly added clauses are checked
        // Problem only arises when new clauses have been added, but FormulaState popped from stack isn't watching them
        std::set<std::pair<unsigned int, unsigned int>> check_watched(FormulaState& fs, bool fix = false, unsigned int start_at = 0) {
          std::set<std::pair<unsigned int, unsigned int>> unit_props;
          for (unsigned int i = start_at; i < formula_.clauses_.size(); i++) {
            if (fs.s[i])
              continue;

            int num_watched = 0;
            unsigned int watch_prop;
            for (auto &l : formula_.clauses_[i].literals_) {
              if (std::find(fs.watched[l.prop_].begin(),
                    fs.watched[l.prop_].end(), i) != fs.watched[l.prop_].end()) {
                num_watched += 1;
                watch_prop = l.prop_;
              }
            }

            if (fix && num_watched < 2) {
              if (verbose_)
                log_info("Fixing watchers for clause", formula_.clauses_[i]);
              
              for (auto &l : formula_.clauses_[i].literals_) {
                if ((!l.negated_ && fs.a[l.prop_] != PropAssignment::False) ||
                    (l.negated_ && fs.a[l.prop_] != PropAssignment::True)) {
                  fs.watched[l.prop_].push_back(i);
                  num_watched += 1;
                  watch_prop = l.prop_;
                }

                if (num_watched == 2) {
                  break;
                }
              }
            }

            if (num_watched == 1) {
              if (verbose_)
                log_info("After fixing, found unit clause");
              unit_props.insert(std::make_pair(i, watch_prop));
            }

            if (num_watched == 0) {
              if (verbose_)
                log_info("After fixing, found conflict clause");
            }
          }

          return unit_props;

          if (!unit_props.empty()) {
            //if (verbose_)
            //  log_info("Running update_watch_and_propagate in check_watched");

            //bool found_conflict = update_watch_and_propagate(fs, unit_props);

            //if (verbose_ && found_conflict)
            //  log_info("Found conflict in check_watched"); 
          }
        }

        bool update_watch_and_propagate(FormulaState& fs, std::set<std::pair<unsigned int, unsigned int>> unit_clauses) {
          if (verbose_) {
            log_info("Doing update_watch_and_propagate for unit clauses");
            for (auto &t : unit_clauses)
              log_info("(", t.first, ",", t.second, ")");
          }

          bool found_conflict = false;

          std::stack<std::pair<unsigned int, unsigned int>> working;
          for (auto &t : unit_clauses)
            working.push(t);

          std::queue<unsigned int> to_watch;
          while (working.size() != 0) {
            bool should_do_unit_preference = true;
            unsigned int unit_clause, unit_prop;
            std::tie(unit_clause, unit_prop) = working.top();
            working.pop();

            if (verbose_)
              log_info("update_watched revealed unit clause", formula_.clauses_[unit_clause], "with", formula_.clauses_[unit_clause].size(fs.a));

            if (!formula_.clauses_[unit_clause].is_unit(fs.a)) {
              //assert(formula_.clauses_[unit_clause].eval(fs.a) == PropAssignment::True);

              if (verbose_)
                log_info("Skipping because clause", formula_.clauses_[unit_clause], "resolved already as", formula_.clauses_[unit_clause].eval(fs.a));

              // Return since we found a conflict in this clause
              if (formula_.clauses_[unit_clause].eval(fs.a) == PropAssignment::False)
                return true;
              
              should_do_unit_preference = false;
            }

            if (should_do_unit_preference) {
              do_unit_preference(fs, unit_clause);

              to_watch.push(unit_prop);
            }

            if (working.size() == 0) {
              while (!to_watch.empty()) {
                unsigned int unit_prop = to_watch.front(); 
                to_watch.pop();
                std::set<std::pair<unsigned int, unsigned int>> tmp_unit_clauses;
                std::pair<int, int> tmp_conflict_clause;
                std::tie(tmp_unit_clauses, tmp_conflict_clause) =
                  update_watched(fs, unit_prop);

                if (verbose_) {
                  log_info("In loop update_watched returned");
                  for (auto &t : tmp_unit_clauses) {
                    log_info("\t(", t.first, ",", t.second, ")");
                  }
                }
              
                if (tmp_conflict_clause.first >= 0) {
                  found_conflict = true;
                  int backjump_level = learn_clause(fs, tmp_conflict_clause.first, tmp_conflict_clause.second);
                  if (backjump_level >= 0) {
                    backjump_ = backjump_level;
                    should_backjump_ = true;
                  }
                  return true;
                } else {
                  for (auto &t : tmp_unit_clauses)
                    working.push(t);
                }
              }
            }
          }

          return found_conflict;
        }

        // The Assignment portion of the return value will be empty unless the
        // DPLLResult part is "Satisfiable"
        std::pair<DPLLResult, Assignment> iterate() {
          if (verbose_) {
            log_info("Before popping, frontier has", frontier_.size(), "elements");
          }

          if (should_backjump_) {
            assert(backjump_ >= 0);

            if (frontier_.size() > 0) {
              if (verbose_) {
                log_info("backjumping to level", backjump_);
                log_info("Decision level at top of stack is", frontier_.top().decision_level);
              }

              //int jumped_decision_level = frontier_.top().decision_level;

              // TODO There's some kind of bug with backjumping which needs to be resolved
              // Backjumping
              // =============
              //int num_jumped = 0;
              //while (jumped_decision_level > backjump_) {
              //  if (verbose_) {
              //    log_info("Popped formula state at level", jumped_decision_level, "from frontier");
              //  }
              //  frontier_.pop();
              //  jumped_decision_level = frontier_.top().decision_level;
              //  num_jumped += 1;

              //  if (verbose_) {
              //    log_info("Decision level at top of stack is", frontier_.top().decision_level);
              //  } 
              //}

              //// Restore branch using trail
              //jumped_decision_level = trail_.top().decision_level;
              //FormulaState pop_fs = trail_.top();
              //while (jumped_decision_level >= backjump_) {
              //  pop_fs = trail_.top();
              //  trail_.pop();
              //  jumped_decision_level = trail_.top().decision_level;
              //  if (verbose_) {
              //    log_info("Decision level at top of trail is", trail_.top().decision_level);
              //  } 
              //}

              //if (num_jumped > 0) {
              //  if (verbose_) {
              //    log_info("Restoring branch at level", pop_fs.decision_level);
              //  }
              //  frontier_.push(pop_fs);
              //}
            }

            should_backjump_ = false;
            backjump_ = -1;
          }

          FormulaState current = frontier_.top();
          frontier_.pop();

          unsigned int num_known_clauses = current.s.size();
          current.s = check_simplification_size(current.s);

          if (verbose_) {
            log_info("Level open is");
            for (unsigned int i = 0; i < current.level_open.size(); i++)
              log_info(i, ":", current.level_open[i]);
          }

          Assignment empty;
          auto e = formula_.eval(current.a, current.s);
          if (e == PropAssignment::True) {
            if (verbose_)
              log_info("Returning on satisfiable");

            return {DPLLResult::Satisfiable, current.a}; 
          } else if (e == PropAssignment::False) {
            if (verbose_)
              log_info("Returning on unsatisfiable");

            return {DPLLResult::Unsatisfiable, empty};
          } 

          // TODO Make check_watched take an index to start at, feed it num_known_clauses
          std::set<std::pair<unsigned int, unsigned int>> check_unit_clauses =
            check_watched(current, true, num_known_clauses);

          if (verbose_)
            log_info("check_watched returned", check_unit_clauses.size(), "unit clauses");

          iterations_ += 1;

          bool found_conflict = false;
          int prev_decision_prop = current.decision_prop;
          //for (unsigned int i = 0; i < formula_.get_num_props(); i++) {
          //  if (current.parents[i] == -2 && current.levels[i] == current.decision_level) {
          //    prev_decision_prop = i;
          //  }
          //}
          assert(frontier_.size() == 0 || prev_decision_prop != -1);
          if (prev_decision_prop != -1) {
            
            bool found_conflict = false;

            std::set<std::pair<unsigned int, unsigned int>> unit_clauses;
            std::pair<int, int> conflict_clause;
            std::tie(unit_clauses, conflict_clause) = update_watched(current, prev_decision_prop);

            unit_clauses.insert(check_unit_clauses.begin(), check_unit_clauses.end());

            if (conflict_clause.first >= 0) {
              found_conflict = true;
              int backjump_level = learn_clause(current, conflict_clause.first, conflict_clause.second);

              if (backjump_level >= 0) {
                backjump_ = backjump_level;
                should_backjump_ = true;
              }
            } else {
              found_conflict = update_watch_and_propagate(current, unit_clauses);
            }
          }

          e = formula_.eval(current.a, current.s);
          if (e == PropAssignment::True) {
            if (verbose_)
              log_info("Returning on satisfiable after update_watch_and_propagate");

            return {DPLLResult::Satisfiable, current.a}; 
          } else if (e == PropAssignment::False) {
            if (verbose_)
              log_info("Returning on unsatisfiable after update_watch_and_propagate");

            return {DPLLResult::Unsatisfiable, empty};
          } 

          if (should_restart_) {
            log_info("Restarting due to learned unit clause");
            restart();
            iterations_ = 0;
            should_restart_ = false;
            return std::make_pair(DPLLResult::Unknown, empty);
          }

          if (verbose_) {
            log_info("assignment is", current.a);
          }
          
          if (found_conflict) {
            if (verbose_)
              log_info("Returning on found conflict");

            return {DPLLResult::Unsatisfiable, empty};
          }

          // If we learn the empty clause, then this formula is unsatisfiable
          if (found_unsat_) {
            if (verbose_)
              log_info("Returning on found unsat");
            while (!frontier_.empty()) {
              frontier_.pop();
            }
            while (!trail_.empty()) {
              trail_.pop();
            }
            return {DPLLResult::Unsatisfiable, empty};
          }

          if (iterations_ > restart_iterations_) {
            log_info("Currently have", formula_.get_num_clauses(), "clauses");
            iterations_ = 0;
            //restart(true);
            //return {DPLLResult::Unknown, empty};
          }

          trail_.push(current);

          if (verbose_) {
            log_info("Trail has size", trail_.size());
          }

          if (!found_conflict) {
            // Remove, just for debugging
            //check_watched(current, true);

            do_splitting_rule(current); 
          }
           
          return {DPLLResult::Unknown, empty};
        }

        friend std::tuple<DPLLResult, Assignment, int> dpll(CNFFormula f,
            PropFunc ph_func, AssignFunc ah_func, const std::chrono::seconds cutoff);

      private:
        CNFFormula formula_;
        std::stack<FormulaState> frontier_;
        std::stack<FormulaState> trail_;
        PropositionHeuristic ph_;
        AssignmentHeuristic ah_; 

        VectorXd vsids_;
        const double vsids_decay_ = 0.5;
        
        int iterations_ = 0;
        const int restart_iterations_ = 1000;

        std::vector<int> learned_usage;

        int max_original_clause_size_;
        int num_original_clauses_;

        bool found_unsat_ = false;
        bool should_restart_ = false;

        bool verbose_ = false;

        int backjump_ = -1;
        bool should_backjump_ = false;

    };

    // Free Functions
    
    // ph_func should return a vector of ranks with the same length as its
    // input vector, and ah_func should return true or false for assignments.
    std::tuple<DPLLResult, Assignment, int> dpll(CNFFormula f,
        PropFunc ph_func, AssignFunc
        ah_func, const std::chrono::seconds cutoff) {

      if (f.get_num_props() == 0) {
        std::valarray<PropAssignment> empty = {};
        return {DPLLResult::Satisfiable, empty, 0};
      }

      DPLLState state(f, PropositionHeuristic(ph_func),
          AssignmentHeuristic(ah_func));

      int calls = 0;
      auto start_time = std::chrono::steady_clock::now();
      while (true) {
        DPLLResult r;
        Assignment a;
        calls += 1;

        std::tie(r, a) = state.iterate();
        //log_info("After iteration, frontier size is", state.frontier_.size());
        if (r == DPLLResult::Satisfiable) {
          return std::make_tuple(r, a, calls);
        } else if (r == DPLLResult::Unsatisfiable && state.frontier_.size() == 0) {
          log_info("UNSAT");
          return std::make_tuple(r, a, calls);
        }

        auto current_time = std::chrono::steady_clock::now();
        auto time_taken =
          std::chrono::duration_cast<std::chrono::seconds>(current_time -
              start_time);
        if (time_taken > cutoff) {
          log_info("Cutoff time of", cutoff.count(), "seconds exceeded");
          return std::make_tuple(r, a, calls);   
        }
      }
    }

    std::ostream& operator<<(std::ostream& os, const DPLLResult& r);


  } // namespace logic
} // namespace cannon

#endif /* ifndef CANNON_LOGIC_DPLL_H */
