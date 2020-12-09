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

#include <cannon/logic/cnf.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::log;

namespace cannon {
  namespace logic {

    using Assignment=std::valarray<PropAssignment>;
    using Simplification=std::valarray<bool>;
    using AssignmentParents=std::valarray<int>;
    using AssignmentLevels=std::valarray<int>;

    struct FormulaState {
      Assignment a;
      Simplification s;
      AssignmentParents parents;
      AssignmentLevels levels;
      std::vector<std::vector<unsigned int>> watched;
      std::vector<bool> level_open;

      int decision_level;
    };

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
        F f_;

    };

    // Heuristic functor for use when applying splitting rule
    template <typename F>
    class AssignmentHeuristic {
      public:
        AssignmentHeuristic() = delete;

        AssignmentHeuristic(F f) : f_(f) {}

        bool choose_assignment(const CNFFormula& form, const Assignment& a, const
            Simplification& s, unsigned int prop, std::vector<std::vector<unsigned int>> watched) {
          return f_(form, a, s, prop, watched);
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

        void restart(bool forget_clauses) {

          while (!frontier_.empty()) {
            frontier_.pop();
          }

          if (forget_clauses) {
            log_info("Before removing learned clauses there are", formula_.clauses_.size());
            std::vector<Clause> rem_clauses;
            for (unsigned int i = num_original_clauses_; i < formula_.clauses_.size(); i++) {
              const Clause& c = formula_.clauses_[i];
              //if (c.literals_.size() >= 4 * max_original_clause_size_ ||
              
              // We explicitly keep unit clauses, since they're so valuable
              if (learned_usage[i - num_original_clauses_] < 1 && c.literals_.size() != 1) 
                rem_clauses.push_back(c);
            }

            std::vector<Clause>::iterator new_end = formula_.clauses_.end();
            for (auto &c : rem_clauses) {
              new_end = std::remove(formula_.clauses_.begin(), new_end, c);
            }
            formula_.clauses_.erase(new_end, formula_.clauses_.end());

            learned_usage = std::vector<int>();
            for (unsigned int i = num_original_clauses_; i < formula_.clauses_.size(); i++) {
              learned_usage.push_back(0);
            }
          }

          // Inject some randomness
          vsids_ += VectorXd::Random(formula_.get_num_props());
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
          std::vector<unsigned int> unit_clauses;

          for (unsigned int i = 0; i < formula_.clauses_.size(); i++) {
            if (s[i])
              continue;

            int num_watched = 0;
            for (auto &l : formula_.clauses_[i].literals_) {
              if (num_watched == 2)
                break;

              if ((!l.negated_ && pld_a[l.prop_] != PropAssignment::False) ||
                  (l.negated_ && pld_a[l.prop_] != PropAssignment::True)) {
                w[l.prop_].push_back(i);
                num_watched += 1;
              }
            }

            if (num_watched == 1) {
              // We have a unit clause
              have_unit_clause = true;
              unit_clauses.push_back(i);
            }

            // We ignore the unsatisfiable case, as that will be resolved in the first iteration of DPLL
          }

          FormulaState tmp_fs = {pld_a, check_simplification_size(s),
            parents_0, levels_0, w, level_open, 0};
          if (have_unit_clause) {
            bool found_conflict = false;
            std::tie(tmp_fs, found_conflict) = do_unit_preference(tmp_fs, unit_clauses);
            frontier_.push(tmp_fs);
          } else
            frontier_.push(tmp_fs); 
        }

        std::pair<FormulaState, bool> update_watched(const FormulaState& fs, unsigned int prop) {
          Assignment a = fs.a;
          Simplification s = check_simplification_size(fs.s);
          AssignmentParents parents = fs.parents;
          AssignmentLevels levels = fs.levels;
          std::vector<std::vector<unsigned int>> w = fs.watched;
          std::vector<bool> level_open = fs.level_open;

          assert(level_open.size() == fs.decision_level + 1);

          std::vector<unsigned int> unit_clauses;
          std::vector<unsigned int> rem_clauses;
          bool have_unit_clause = false;
          bool found_conflict = false;

          for (unsigned int c : w[prop]) {
            if (s[c])
              continue;

            bool watch_new = false;

            for (auto &l : formula_.clauses_[c].literals_) {
              if (l.prop_ != prop)
                continue;

              // Past here, l.prop_ == prop

              // If the assignment doesn't evaluate to false here, don't watch anything new
              if ((l.negated_ && a[prop] == PropAssignment::False) ||
                  (!l.negated_ && a[prop] == PropAssignment::True) || a[prop]
                  == PropAssignment::Unassigned)
                break;

              // Otherwise, we need to watch something new
              rem_clauses.push_back(c);
              watch_new = true;
            }

            if (watch_new) {
              int num_watched = 0;
              for (auto &l : formula_.clauses_[c].literals_) {
                if (l.prop_ == prop)
                  continue;

                if (num_watched == 2) {
                  break;
                }

                if (std::find(w[l.prop_].begin(), w[l.prop_].end(), c) != w[l.prop_].end()) {
                  // This literal is already watched, so move on
                  num_watched += 1;
                  continue;
                } else {
                  if (a[l.prop_] == PropAssignment::Unassigned || (l.negated_
                        && a[l.prop_] == PropAssignment::False) || (!l.negated_
                        && a[l.prop_] == PropAssignment::True)) {
                    w[l.prop_].push_back(c);
                    num_watched += 1;
                  }
                }
              }

              if (num_watched == 1) {
                // We have a unit clause
                have_unit_clause = true; 
                unit_clauses.push_back(c);
              }

              if (num_watched == 0 && !found_conflict) {
                // We have a conflict, so we can terminate
                if (parents[prop] == -2) {
                  //log_info("Prop", prop, "resulting from splitting",
                  //    "led to a conflict in clause", 
                  //    formula_.clauses_[c]);
                } else if (parents[prop] >= 0) {
                  if (formula_.clauses_[parents[prop]] == formula_.clauses_[c]) {
                    log_info("ALERT");
                  }

                  //log_info("Prop", prop, "set to", a[prop], "with parent clause",
                  //    formula_.clauses_[parents[prop]],"led to a conflict in clause", 
                  //    formula_.clauses_[c]);

                  //log_info("Conflict clause",
                  //    resolve(formula_.clauses_[parents[prop]],
                  //      formula_.clauses_[c], prop));

                  if (parents[prop] > num_original_clauses_) {
                    learned_usage[parents[prop] - num_original_clauses_] += 1;
                  }
                  if (c > num_original_clauses_) {
                    learned_usage[c - num_original_clauses_] += 1;
                  }

                  // Do all possible resolutions
                  Clause learned_c = formula_.clauses_[c];
                  std::queue<Literal> resolve_literals;
                  int num_current_level = 0;
                  for (auto &l : learned_c.literals_) {
                    if (parents[l.prop_] >= 0 && levels[l.prop_] == fs.decision_level)
                      resolve_literals.push(l);
                    
                    if (levels[l.prop_] == fs.decision_level)
                      num_current_level += 1;
                  }

                  while (num_current_level > 1) {
                    while (resolve_literals.size() != 0) {
                      Literal l = resolve_literals.front();
                      resolve_literals.pop();

                      //log_info("Resolving", learned_c, "on", l, "with parent",
                      //    formula_.clauses_[parents[l.prop_]]);
                      learned_c = resolve(learned_c, formula_.clauses_[parents[l.prop_]], l.prop_);

                      if (parents[l.prop_] > num_original_clauses_) {
                        learned_usage[parents[l.prop_] - num_original_clauses_] += 1;
                      }

                      num_current_level = 0;
                      for (auto &ll : learned_c.literals_) {
                        if (levels[ll.prop_] == fs.decision_level) {
                          num_current_level += 1;
                        }
                      }

                      if (num_current_level == 1) {
                        break;
                      } 
                    }
                    num_current_level = 0;
                    for (auto &ll : learned_c.literals_) {
                      if (parents[ll.prop_] >= 0 && levels[ll.prop_] == fs.decision_level)
                        resolve_literals.push(ll);

                      if (levels[ll.prop_] == fs.decision_level)
                        num_current_level += 1;
                    }
                  }

                  //log_info("Current level is", fs.decision_level);
                  //log_info("Learned clause", learned_c);
                  //log_info("Clause levels are");
                  //for (auto &l : learned_c.literals_) {
                  //  log_info("\t", levels[l.prop_], ",", level_open[levels[l.prop_]]);  
                  //}

                  // Exponential moving average vsids
                  vsids_ *= (1.0 - vsids_decay_);

                  for (auto &l : learned_c.literals_) {
                    vsids_[l.prop_] += vsids_decay_;
                  }
                  formula_.add_clause(learned_c);

                  // Keep track of usage of learned clauses
                  learned_usage.push_back(0);

                  //log_info("Current level is", fs.decision_level);
                  //log_info("Learned clause", learned_c);
                  //log_info("Clause levels are");
                  //for (auto &l : learned_c.literals_) {
                  //  log_info("\t", levels[l.prop_], ",", level_open[levels[l.prop_]]);  
                  //}

                  if (learned_c.literals_.size() == 1) {
                    log_info("Learned unit clause", learned_c);
                    should_restart_ = true;
                  }

                  if (learned_c.literals_.size() == 0) {
                    log_info("Learned empty clause (conflict)", learned_c);
                    found_unsat_ = true;
                  }

                  //log_info("Formula is now", formula_);

                  int new_clause_num = formula_.get_num_clauses();
                  int num_learned_watched = 0;
                  int max_decision_level = 0;
                  for (auto &l : learned_c.literals_) {

                    // Keep track of latest decision level contributing to this conflict
                    if (levels[l.prop_] > max_decision_level &&
                        level_open[levels[l.prop_] && levels[l.prop_] != fs.decision_level]) 
                      max_decision_level = levels[l.prop_];

                    if (num_learned_watched < 2) {
                      if ((!l.negated_ && a[l.prop_] != PropAssignment::False)
                          || (l.negated_ && a[l.prop_] != PropAssignment::True)) {
                        w[l.prop_].push_back(new_clause_num);
                        num_learned_watched += 1;
                      }
                    }
                  }

                  // Backjump by unwinding the stack of formulas states
                  if (frontier_.size() > 0 && max_decision_level != 0) {
                    //log_info("max_decision_level is", max_decision_level);
                    //log_info("Decision level at top of stack is", frontier_.top().decision_level);
                    int jumped_decision_level = frontier_.top().decision_level;

                    int num_jumped = 0;
                    while (jumped_decision_level > max_decision_level) {
                      frontier_.pop();
                      //log_info("Popped formula state at level", jumped_decision_level, "from frontier");
                      jumped_decision_level = frontier_.top().decision_level;
                      //log_info("Decision level at top of stack is", frontier_.top().decision_level);
                      num_jumped += 1;
                    }

                    // Push restored branch
                    Assignment restored_a = Assignment(PropAssignment::Unassigned, formula_.get_num_props());
                    Simplification restored_s = Simplification(false, formula_.get_num_clauses());
                    AssignmentParents restored_parents = AssignmentParents(-1, formula_.get_num_props());
                    AssignmentLevels restored_levels = AssignmentLevels(-1, formula_.get_num_props());
                    std::vector<std::vector<unsigned int>> restored_w;
                    for (unsigned int i = 0; i < formula_.get_num_props(); i++) {
                      restored_w.emplace_back();
                    }

                    std::vector<bool> restored_level_open;
                    for (int i = 0; i <= max_decision_level; i++) {
                      restored_level_open.push_back(level_open[i]);
                    }

                    for (int i = 0; i < formula_.get_num_props(); i++) {
                      if (levels[i] <= max_decision_level) {
                        restored_a[i] = a[i];
                        restored_parents[i] = parents[i];
                        restored_levels[i] = levels[i];
                      }
                    }

                    restored_s = formula_.simplify(restored_a, restored_s);

                    for (unsigned int i = 0; i < formula_.clauses_.size(); i++) {
                      int num_watched = 0;
                      for (auto &l : formula_.clauses_[i].literals_) {
                        if (num_watched == 2)
                          break;

                        if ((!l.negated_ && restored_a[l.prop_] != PropAssignment::False) ||
                            (l.negated_ && restored_a[l.prop_] != PropAssignment::True)) {
                          restored_w[l.prop_].push_back(i);
                          num_watched += 1;
                        } 
                      }
                    }

                    //log_info("Restoring branch at level", max_decision_level);
                    FormulaState tmp_fs = {restored_a, check_simplification_size(restored_s),
                      restored_parents, restored_levels, restored_w,
                      restored_level_open, max_decision_level};
                    FormulaState push_fs;
                    std::vector<unsigned int> learned_unit_clause;
                    learned_unit_clause.push_back(formula_.clauses_.size() - 1);
                    std::tie(push_fs, found_conflict) = do_unit_preference(tmp_fs, learned_unit_clause);
                    frontier_.push(push_fs);

                    //if (num_jumped > 0)
                    //  log_info("Jumped back", num_jumped, "levels");
                  }
                } else {
                  throw std::runtime_error("Conflict arose from unset proposition");
                }

                found_conflict = true;
              }
            }
          }

          for (unsigned int c : rem_clauses) {
            auto end = std::remove(w[prop].begin(), w[prop].end(), c);
            w[prop].erase(end, w[prop].end());
          }

          FormulaState tmp_fs = {a, check_simplification_size(s), parents,
            levels, w, fs.level_open, fs.decision_level};
          if (have_unit_clause && !found_conflict) {
            FormulaState tmp2 = tmp_fs;
            std::tie(tmp_fs, found_conflict) = do_unit_preference(tmp2, unit_clauses);
            return std::make_pair(tmp_fs, found_conflict);
          } else
            return std::make_pair(tmp_fs, found_conflict);
        }

        void do_splitting_rule(FormulaState& fs) {
          //log_info("Doing splitting rule at decision level", fs.decision_level + 1);
          Assignment a = fs.a;
          Simplification s = check_simplification_size(fs.s);
          AssignmentParents parents = fs.parents;
          AssignmentLevels levels = fs.levels;
          std::vector<std::vector<unsigned int>> w = fs.watched;
          std::vector<bool> level_open = fs.level_open;


          // Create vector of all propositions
          std::vector<unsigned int> all_props = formula_.get_props(a, s);
          if (all_props.size() == 0) {
            log_info("All props is empty");
            return;
          }

          unsigned int prop = ph_.choose_prop(formula_, a, s, all_props, w, vsids_);
          bool first_assignment = ah_.choose_assignment(formula_, a, s, prop, w);

          assert(a[prop] == PropAssignment::Unassigned);
          //log_info("Splitting on", prop);
          
          // First branch
          Assignment split_first_a(a);
          split_first_a[prop] = first_assignment ? PropAssignment::True : PropAssignment::False;
          Simplification split_first_s = check_simplification_size(formula_.simplify(split_first_a, s));

          AssignmentParents split_first_parents(parents);
          AssignmentLevels split_first_levels(levels);

          split_first_parents[prop] = -2;
          split_first_levels[prop] = fs.decision_level + 1;

          std::vector<bool> split_first_level_open(level_open);
          split_first_level_open.push_back(true);

          FormulaState unsimplified_first_fs = {split_first_a, split_first_s,
            split_first_parents, split_first_levels, w, split_first_level_open,
            fs.decision_level + 1};
          //FormulaState unsimplified_first_fs = update_watched(tmp_fs, prop);

          // Second branch
          Assignment split_second_a(a);
          split_second_a[prop] = first_assignment ? PropAssignment::False : PropAssignment::True;
          Simplification split_second_s = check_simplification_size(formula_.simplify(split_second_a, s));

          AssignmentParents split_second_parents(parents);
          AssignmentLevels split_second_levels(levels);

          split_second_parents[prop] = -2;
          split_second_levels[prop] = fs.decision_level + 1;

          std::vector<bool> split_second_level_open(level_open);
          split_second_level_open.push_back(false);
          FormulaState unsimplified_second_fs = {split_second_a, split_second_s, split_second_parents,
            split_second_levels, w, split_second_level_open, fs.decision_level
              + 1};
          //FormulaState unsimplified_second_fs = update_watched(tmp_fs, prop);

          // Push the second branch to be evaluated first, since we're using a stack
          assert(split_first_a != split_second_a);
          assert(split_first_a != a);
          assert(split_second_a != a);

          // Update watched lists
          //log_info("Pushing branches at decision level", unsimplified_second_fs.decision_level, "on stack");
          frontier_.push({unsimplified_second_fs.a, split_second_s,
              unsimplified_second_fs.parents, unsimplified_second_fs.levels,
              unsimplified_second_fs.watched, unsimplified_second_fs.level_open,
              unsimplified_second_fs.decision_level});
          frontier_.push({unsimplified_first_fs.a, split_first_s,
              unsimplified_first_fs.parents, unsimplified_first_fs.levels,
              unsimplified_first_fs.watched, unsimplified_first_fs.level_open,
              unsimplified_first_fs.decision_level});

          //log_info("Frontier has size", frontier_.size());
        }

        std::pair<FormulaState, bool> do_unit_preference(FormulaState& fs, std::vector<unsigned
            int> unit_clauses) {
          //log_info("Doing unit preference with", unit_clauses.size(), "provided clauses");

          Assignment a = fs.a;
          AssignmentParents parents = fs.parents;
          AssignmentLevels levels = fs.levels;
          Simplification s = check_simplification_size(fs.s);
          std::vector<std::vector<unsigned int>> w = fs.watched;

          //if (unit_clauses.size() == 0) {
          //  units = formula_.get_unit_clause_props(a, s, w);
          //} else {
          //  units = formula_.get_unit_clause_props(a, s, unit_clauses);
          //}

          std::vector<std::tuple<unsigned int, bool, int>> units =
            formula_.get_unit_clause_props(a, s, unit_clauses);

          if (units.size() == 0) {
            return std::make_pair(fs, false);
          }

          std::set<unsigned int> unit_props;
          std::map<unsigned int, int> prop_parents;
          std::map<unsigned int, bool> prop_neg;
          for (auto& t : units) {
            unit_props.insert(std::get<0>(t));
            prop_parents[std::get<0>(t)] = std::get<2>(t);
            prop_neg[std::get<0>(t)] = std::get<1>(t);
          }

          std::vector<unsigned int> unit_prop_vec;
          unit_prop_vec.reserve(unit_props.size());
          unit_prop_vec.insert(unit_prop_vec.end(), unit_props.begin(), unit_props.end());

          //log_info("Unit_prop_vec is");
          //for (auto &t : unit_prop_vec)
          //  log_info("\t", t);

          // Resolve all unit clauses
          while (unit_prop_vec.size() != 0) {
            unsigned int prop;

            prop = unit_prop_vec[0];

            //log_info("Doing unit preference on", prop);
            Assignment unit_a(a);
            AssignmentParents unit_parents(parents);
            AssignmentLevels unit_levels(levels);

            auto idx = std::find_if(units.begin(), units.end(), 
                [prop](const std::tuple<unsigned int, bool, int>& o){ return std::get<0>(o) == prop; });

            // Either the prop or its negated version must be a literal
            assert(idx != units.end());
            assert(unit_a[prop] == PropAssignment::Unassigned);
            assert(parents[prop] == -1);
            assert(levels[prop] == -1);

            bool negated = prop_neg[prop];
            if (negated) {
              unit_a[prop] = PropAssignment::False; 
            } else {
              unit_a[prop] = PropAssignment::True;
            } 
            unit_parents[prop] = prop_parents[prop];
            unit_levels[prop] = fs.decision_level;

            //log_info("Prop", prop, "was set to", unit_a[prop], "with parent", formula_.clauses_[unit_parents[prop]]);

            assert(unit_a != a);
            Simplification unit_s = check_simplification_size(formula_.simplify(unit_a, s));

            a = unit_a;
            s = unit_s;
            parents = unit_parents;
            levels = unit_levels;

            auto end = std::remove(unit_prop_vec.begin(), unit_prop_vec.end(), prop);
            unit_prop_vec.erase(end, unit_prop_vec.end());
          }

          bool found_conflict = false;
          for (auto &prop : unit_props) {
            FormulaState tmp_fs = {a, check_simplification_size(s), parents,
              levels, w, fs.level_open, fs.decision_level};
            FormulaState unsimplified_fs;
            std::tie(unsimplified_fs, found_conflict) = update_watched(tmp_fs, prop);

            a = unsimplified_fs.a;
            s = unsimplified_fs.s;
            parents = unsimplified_fs.parents;
            levels = unsimplified_fs.levels;
            w = unsimplified_fs.watched;
            if (found_conflict)
              break;
          }

          //log_info("Returning assignment", a);
          FormulaState tmp_fs = {a, check_simplification_size(s), parents,
            levels, w, fs.level_open, fs.decision_level};
          return std::make_pair(tmp_fs, found_conflict);
        }

        void check_watched(const FormulaState& fs) {
          for (unsigned int i = 0; i < formula_.clauses_.size(); i++) {
            if (fs.s[i])
              continue;

            int num_watched = 0;
            for (auto &l : formula_.clauses_[i].literals_) {
              if (std::find(fs.watched[l.prop_].begin(),
                    fs.watched[l.prop_].end(), i) != fs.watched[l.prop_].end()) {
                num_watched += 1;
              }
            }

            if (num_watched != 2) {
              log_info("Clause", formula_.clauses_[i], " (", i, ") not watched correctly");
              log_info("Had", num_watched, "watchers");
            }
          }
        }

        // The Assignment portion of the return value will be empty unless the
        // DPLLResult part is "Satisfiable"
        std::pair<DPLLResult, Assignment> iterate() {
          //log_info("Before popping, frontier has", frontier_.size(), "elements");
          FormulaState current = frontier_.top();
          frontier_.pop();


          iterations_ += 1;

          //log_info("Popped assignment");
          //for (auto &t : a)
          //  log_info("\t", t);

          bool found_conflict = false;
          int prev_decision_prop = -1;
          for (unsigned int i = 0; i < formula_.get_num_props(); i++) {
            if (current.parents[i] == -2 && current.levels[i] == current.decision_level) {
              prev_decision_prop = i;
            }
          }
          assert(frontier_.size() == 0 || prev_decision_prop != -1);
          if (frontier_.size() != 0) {
            //log_info("Doing update_watched for prop", prev_decision_prop);
            FormulaState tmp_fs = current;
            std::tie(current, found_conflict) = update_watched(tmp_fs, prev_decision_prop);
          }

          Assignment a = current.a;
          Simplification s = check_simplification_size(current.s);

          //log_info("assignment is", a);
          Assignment empty;
          auto e = formula_.eval(a, s);
          if (e == PropAssignment::True) {
            return {DPLLResult::Satisfiable, a}; 
          } else if (e == PropAssignment::False) {
            //log_info("Returning on unsatisfiable");
            return {DPLLResult::Unsatisfiable, empty};
          } else if (found_conflict) {
            log_info("Returning on found conflict");
            return {DPLLResult::Unsatisfiable, empty};
          }

          // If we learn the empty clause, then this formula is unsatisfiable
          if (found_unsat_) {
            while (!frontier_.empty()) {
              frontier_.pop();
            }
            return {DPLLResult::Unsatisfiable, empty};
          }

          if (should_restart_) {
            log_info("Restarting due to learned unit clause");
            restart(false);
            iterations_ = 0;
            should_restart_ = false;
            return {DPLLResult::Unknown, empty};
          }

          // TODO This logic shouldn't be necessary anymore, as we do unit
          // preference stuff when updating the watched list
          
          // Only do splitting rule if there are no unit clauses
          //if (formula_.get_unit_clause_props(a, s).size() == 0) {
          //  do_splitting_rule(current);
          //}
          
          // Add unit clauses
          //frontier_.push(do_unit_preference(current));
          
          
          //if (iterations_ > restart_iterations_) {
          //  restart(true);
          //  iterations_ = 0;
          //  return {DPLLResult::Unknown, empty};
          //}

          if (!found_conflict) {
            // Remove, just for debugging
            // TODO Resume here; 0 watcher issue when restarting is turned off
            check_watched(current);

            do_splitting_rule(current); 
          }
           
          return {DPLLResult::Unknown, empty};
        }

        friend std::tuple<DPLLResult, Assignment, int> dpll<F, G>(CNFFormula f,
            F ph_func, G ah_func, const std::chrono::seconds cutoff);

      private:
        CNFFormula formula_;
        std::stack<FormulaState> frontier_;
        PropositionHeuristic<F> ph_;
        AssignmentHeuristic<G> ah_; 

        VectorXd vsids_;
        const double vsids_decay_ = 0.5;
        
        int iterations_ = 0;
        const int restart_iterations_ = 1000;

        std::vector<int> learned_usage;

        int max_original_clause_size_;
        int num_original_clauses_;

        bool found_unsat_ = false;
        bool should_restart_ = false;

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
        //log_info("After iteration, frontier size is", state.frontier_.size());
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
