#include <cannon/logic/dpll.hpp>

#include <cassert>

#include <cannon/log/registry.hpp>

using namespace cannon::log;
using namespace cannon::logic;


void FormulaState::write_graph() {
  std::ofstream os("tmp.dot");
  os << "digraph dpll_state {" << std::endl;

  // TODO Can write to a temp file and wait for user input to continue.
  // Also, only write graph when a clause is learned, as that's the
  // inflection point we care about.

  // TODO Write graph (check assignment, level_open, etc.)
  // Arrow back from conflict level to branch where backjumping will restore

  os << "}" << std::endl;

  do {
    std::cout << "Press Enter to continue..." << std::endl;
  } while (std::cin.get() != '\n');

  return;
}


std::ostream& cannon::logic::operator<<(std::ostream& os, const DPLLResult& r) {
  if (r == DPLLResult::Satisfiable)
    os << "Satisfiable";
  else if (r == DPLLResult::Unknown)
    os << "Unknown";
  else
    os << "Unsatisfiable";

  return os;
}

void DPLLState::restart() {
  while (!frontier_.empty()) {
    frontier_.pop();
  }

  // TODO Straight up copy from Mini-SAT reduceDB()
  // reduce_clauses();

  // Inject some randomness
  //vsids_ = VectorXd::Random(formula_.get_num_props());
  do_preprocessing();
}

Simplification DPLLState::check_simplification_size(const Simplification& s) {
  assert(s.size() <= formula_.get_num_clauses());

  if (s.size() == formula_.get_num_clauses())
    return s;

  Simplification ret_s(false, formula_.get_num_clauses());
  ret_s[std::slice(0, s.size(), 1)] = s;

  return ret_s;
}

Assignment DPLLState::do_pure_literal_deletion(const Assignment& a_0, const Simplification& s_0) {
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

void DPLLState::do_preprocessing() {
  Assignment a_0 = Assignment(PropAssignment::Unassigned, formula_.get_num_props());
  AssignmentParents parents_0 = AssignmentParents(-1, formula_.get_num_props());
  AssignmentLevels levels_0 = AssignmentLevels(-1, formula_.get_num_props());
  Simplification s_0 = Simplification(false, formula_.get_num_clauses());

  Assignment pld_a = do_pure_literal_deletion(a_0, s_0);
  Simplification s = formula_.simplify(pld_a, s_0);

  watched = std::vector<std::vector<unsigned int>>();
  for (unsigned int i = 0; i < formula_.get_num_props(); i++) {
    watched.emplace_back();
  }

  clause_watched = std::vector<std::map<unsigned int, bool>>();
  for (unsigned int i = 0; i < formula_.get_num_clauses(); i++) {
    clause_watched.emplace_back();
  }

  std::vector<bool> level_open;
  level_open.push_back(false);

  bool have_unit_clause = false;
  std::set<std::pair<unsigned int, unsigned int>> unit_props;

  for (unsigned int i = 0; i < formula_.clauses_.size(); i++) {
    int num_watched = 0;
    unsigned int watch_prop = (*formula_.clauses_[i].literals_.begin()).prop_;
    for (auto &l : formula_.clauses_[i].literals_) {

      if ((!l.negated_ && pld_a[l.prop_] != PropAssignment::False) ||
          (l.negated_ && pld_a[l.prop_] != PropAssignment::True)) {
        watched[l.prop_].push_back(i);
        num_watched += 1;
        watch_prop = l.prop_;

        if (num_watched < 2) {
          clause_watched[i][l.prop_] = true; 
        }
      } else {
        clause_watched[i][l.prop_] = false;
      }
    }

    if (num_watched == 1) {
      // We have a unit clause

      if (formula_.clauses_[i].size(pld_a) == 1) {
        have_unit_clause = true;
        unit_props.insert(std::make_pair(i, watch_prop));
      }
    }

    // We ignore the unsatisfiable case, as that will be resolved in the first iteration of DPLL
  }

  auto tmp_fs = std::make_shared<FormulaState>(pld_a,
      check_simplification_size(s), parents_0, levels_0, level_open, 0,
      -1);

  if (have_unit_clause) {
    update_watch_and_propagate(tmp_fs, unit_props);
    frontier_.push(tmp_fs);
  } else
    frontier_.push(tmp_fs); 
}

void DPLLState::do_all_resolutions(std::shared_ptr<FormulaState> fs, Clause& c) {
  Literal resolve_literal = *(c.literals_.begin());
  unsigned int max_level = 0;
  bool can_resolve = false;
  unsigned int num_current_level = 0;

  for (auto &l : c.literals_) {
    if ((fs->parents[l.prop_] >= 0) && (fs->levels[l.prop_] >= (int)max_level)) {
      resolve_literal = l;
      max_level = fs->levels[resolve_literal.prop_];
      can_resolve = true;
    }

    if (fs->levels[l.prop_] == fs->decision_level)
      num_current_level += 1;
  }

  // Exponential moving average vsids
  vsids_ *= vsids_decay_;

  // TODO Also decay learned_usage_, then use with LBD to sort clauses for
  // deletion.  Make sure not to delete clauses that are parents of props in
  // the current level, binary clauses, or clauses with lbd <= 2. To implement
  // this will need to make it so that clause ordering can change.

  while (can_resolve && num_current_level > 1) {
    can_resolve = false;

    resolve(c, formula_.clauses_[fs->parents[resolve_literal.prop_]],
        resolve_literal.prop_);

    for (auto &l : formula_.clauses_[fs->parents[resolve_literal.prop_]].literals_) {
      vsids_[l.prop_] += 1.0;
    }

    if (fs->parents[resolve_literal.prop_] > (int)num_original_clauses_) {
      learned_usage[fs->parents[resolve_literal.prop_] - num_original_clauses_] += 1;
    }

    resolve_literal = *(c.literals_.begin());
    unsigned int max_level = 0;
    for (auto &l : c.literals_) {
      if ((fs->parents[l.prop_] >= 0) && (fs->levels[l.prop_] >= (int)max_level)) {
        resolve_literal = l;
        max_level = fs->levels[resolve_literal.prop_];
      }
      
      if (fs->parents[l.prop_] >= 0)
        can_resolve = true;

      if (fs->levels[l.prop_] == fs->decision_level)
        num_current_level += 1;
    }
  }
}

int DPLLState::compute_lbd(std::shared_ptr<FormulaState> fs, const Clause& c) {
  std::set<unsigned int> c_levels;

  for (const auto& l : c.literals_) {
    c_levels.insert(fs->levels[l.prop_]);
  }

  return c_levels.size();
}

int DPLLState::learn_clause(std::shared_ptr<FormulaState> fs, unsigned int c, unsigned int prop) {
  // We have a conflict, so we can terminate

  assert(formula_.clauses_[c].eval(fs->a) == PropAssignment::False);

  if (fs->parents[prop] != -1) {
    if (fs->parents[prop] > (int)num_original_clauses_) {
      learned_usage[fs->parents[prop] - num_original_clauses_] += 1;
    }
    if (c > num_original_clauses_) {
      learned_usage[c - num_original_clauses_] += 1;
    }

    // Do all possible resolutions
    Clause learned_c = formula_.clauses_[c];
    do_all_resolutions(fs, learned_c);

    int lbd = compute_lbd(fs, learned_c);
    lbd_ema_ = (lbd_ema_decay_ * lbd_ema_) + ((1.0 - lbd_ema_decay_) * lbd);
    
    if (lbd > lbd_ema_) {
      vsids_decay_ = 0.75;
    } else {
      vsids_decay_ = 0.99;
    }

    int prev_clauses = formula_.get_num_clauses();
    formula_.add_clause(std::move(learned_c));
    int post_clauses = formula_.get_num_clauses();

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
    int min_closed_level = formula_.get_num_props();
    for (auto &l : learned_c.literals_) {
      // Keep track of latest decision level contributing to this conflict
      if ((fs->levels[l.prop_] > max_decision_level) &&
          fs->level_open[fs->levels[l.prop_]]) {
        max_decision_level = fs->levels[l.prop_];
      } else if (!fs->level_open[fs->levels[l.prop_]] && (fs->levels[l.prop_] <= min_closed_level)) {
        min_closed_level = fs->levels[l.prop_];
      }
    }

    if (prev_clauses != post_clauses) {
      learned_usage.push_back(1);
      int new_clause_num = formula_.get_num_clauses() - 1;
      clause_watched.emplace_back();
      
      if (learned_c.size() > 1) {
        unsigned int w1 = 0;
        unsigned int w2 = 0;

        unsigned int w2_level = 0;

        for (auto &l : learned_c.literals_) {
          if (fs->levels[l.prop_] >= (int)w2_level) {
            w1 = w2;
            w2 = l.prop_;
            w2_level = fs->levels[l.prop_];
          }

          clause_watched[new_clause_num][l.prop_] = false;
        }

        // Watch the literals with the highest decision level

        watched[w1].push_back(new_clause_num);
        watched[w2].push_back(new_clause_num);

        clause_watched[new_clause_num][w1] = true;
        clause_watched[new_clause_num][w2] = true;

        num_learned_watched = 2;
      } else if (learned_c.size() == 1) {
        watched[(*learned_c.literals_.begin()).prop_].push_back(new_clause_num);
        clause_watched[new_clause_num][(*learned_c.literals_.begin()).prop_] = true;
        num_learned_watched = 1;
      }

      assert(learned_c.size() == 0 || learned_c.size() == 1 || num_learned_watched == 2);
    }

    // Backjump by unwinding the stack of formulas states
    if (max_decision_level != -1) {
      return max_decision_level;
    } else {
      return min_closed_level;
    }

  } else {
    throw std::runtime_error("Conflict arose from unset proposition");
    return -1;
  }

  return -1;
}

std::pair<std::set<std::pair<unsigned int, unsigned int>>,
  std::pair<int, int>> DPLLState::update_watched(std::shared_ptr<FormulaState> fs, unsigned int prop) {
  assert(fs->level_open.size() == (unsigned int)(fs->decision_level + 1));

  std::set<std::pair<unsigned int, unsigned int>> unit_clauses;
  std::pair<int, int> conflict_clause = std::make_pair(-1, -1);

  std::vector<unsigned int> rem_clauses;

  for (const unsigned int& c : watched[prop]) {
    if (fs->s[c])
      continue;

    bool watch_new = false;

    for (const auto &l : formula_.clauses_[c].literals_) {
      if (l.prop_ != prop)
        continue;

      // Past here, l.prop_ == prop

      // If the assignment doesn't evaluate to false here, don't watch anything new
      if ((l.negated_ && (fs->a[prop] == PropAssignment::False)) ||
          (!l.negated_ && (fs->a[prop] == PropAssignment::True)) || fs->a[prop]
          == PropAssignment::Unassigned)
        break;

      // Otherwise, we need to watch something new
      watch_new = true;
    }

    if (watch_new) {
      int num_watched = 0;
      unsigned int watch_prop = (*formula_.clauses_[c].literals_.begin()).prop_;

      std::vector<Literal> unwatched_literals;
      for (const auto &l : formula_.clauses_[c].literals_) {
        if (l.prop_ == prop)
          continue;

        if (num_watched == 2) {
          break;
        }

        if (clause_watched[c][l.prop_]) {
          // This literal is already watched, so move on
          num_watched += 1;

          watch_prop = l.prop_;
        } else {
          unwatched_literals.push_back(l);
        }
      }

      for (const auto &l : unwatched_literals) {
        if (l.prop_ == prop)
          continue;

        if (num_watched == 2) {
          break;
        }

        if (fs->a[l.prop_] == PropAssignment::Unassigned || (l.negated_
              && (fs->a[l.prop_] == PropAssignment::False)) || (!l.negated_
              && (fs->a[l.prop_] == PropAssignment::True))) {
          watched[l.prop_].push_back(c);
          clause_watched[c][l.prop_] = true;

          watch_prop = l.prop_;
          num_watched += 1;
        }
      }
      
      if (num_watched == 2) {
        // We only remove if num_watched is two, as otherwise we are either backtracking or propagating a unit clause
        rem_clauses.push_back(c);
      }

      if (num_watched == 1) {
        // We may have a unit clause
        if (formula_.clauses_[c].size(fs->a) == 1)
          unit_clauses.insert(std::make_pair(c, watch_prop));
      }

      if (num_watched == 0) {
        conflict_clause = std::make_pair(c, prop);
        break;
      }
    }
  }

  for (const unsigned int& c : rem_clauses) {
    watched[prop].erase(std::remove(watched[prop].begin(), watched[prop].end(), c), watched[prop].end());
    clause_watched[c][prop] = false;
  }

  return std::make_pair(unit_clauses, conflict_clause);
}

bool DPLLState::do_splitting_rule(const std::shared_ptr<FormulaState> fs) {
  // Create vector of all propositions
  std::vector<unsigned int> all_props = fs->untried_props;
  if (all_props.size() == 0) {
    return false;
  } 

  unsigned int prop = ph_.choose_prop(*this, fs->a, fs->s, all_props);
  bool first_assignment = ah_.choose_assignment(formula_, fs->a, fs->s, prop, watched);

  assert(fs->a[prop] == PropAssignment::Unassigned);
  fs->untried_props.erase(std::remove(fs->untried_props.begin(),
        fs->untried_props.end(), prop), fs->untried_props.end());
  
  // First branch
  Assignment split_first_a(fs->a);
  split_first_a[prop] = first_assignment ? PropAssignment::True : PropAssignment::False;
  Simplification split_first_s = formula_.simplify(split_first_a, fs->s);

  AssignmentParents split_first_parents(fs->parents);
  AssignmentLevels split_first_levels(fs->levels);

  split_first_parents[prop] = -2;
  split_first_levels[prop] = fs->decision_level + 1;

  std::vector<bool> split_first_level_open(fs->level_open);
  split_first_level_open.push_back(true);

  int tmp_prop = prop;
  auto unsimplified_first_fs =
    std::make_shared<FormulaState>(split_first_a, split_first_s,
        split_first_parents, split_first_levels,
        split_first_level_open, fs->decision_level + 1, tmp_prop);

  // Second branch
  Assignment split_second_a(fs->a);
  split_second_a[prop] = first_assignment ? PropAssignment::False : PropAssignment::True;
  Simplification split_second_s = formula_.simplify(split_second_a, fs->s);

  AssignmentParents split_second_parents(fs->parents);
  AssignmentLevels split_second_levels(fs->levels);

  split_second_parents[prop] = -2;
  split_second_levels[prop] = fs->decision_level + 1;

  std::vector<bool> split_second_level_open(fs->level_open);
  split_second_level_open.push_back(false);
  auto unsimplified_second_fs =
    std::make_shared<FormulaState>(split_second_a, split_second_s,
        split_second_parents, split_second_levels,
        split_second_level_open, fs->decision_level + 1, tmp_prop);

  assert(split_first_a != split_second_a);
  assert(split_first_a != fs->a);
  assert(split_second_a != fs->a);

  // Push the second branch to be evaluated first, since we're using a stack
  frontier_.push(unsimplified_second_fs);
  frontier_.push(unsimplified_first_fs);

  return true;
}

void DPLLState::do_unit_preference(std::shared_ptr<FormulaState> fs, unsigned int unit_clause) {
  Clause& unit_c = formula_.clauses_[unit_clause];
  assert(unit_c.is_unit(fs->a));

  unsigned int unit_prop = unit_c.get_unit_prop(fs->a);
  unsigned int unit_prop_negated = unit_c.get_unit_negated(fs->a);

  // Either the prop or its negated version must be a literal
  assert(fs->a[unit_prop] == PropAssignment::Unassigned);
  assert(fs->parents[unit_prop] == -1);
  assert(fs->levels[unit_prop] == -1);

  if (unit_prop_negated) {
    fs->a[unit_prop] = PropAssignment::False; 
  } else {
    fs->a[unit_prop] = PropAssignment::True;
  } 
  fs->parents[unit_prop] = unit_clause;
  fs->levels[unit_prop] = fs->decision_level;
  fs->untried_props.erase(std::remove(fs->untried_props.begin(),
        fs->untried_props.end(), unit_prop), fs->untried_props.end());

  // Taking a lot of time, switching to just setting
  //fs->s = formula_.simplify(fs->a, fs->s);
  //assert(fs->s[unit_clause]);
  fs->s[unit_clause] = true;
}

bool DPLLState::update_watch_and_propagate(std::shared_ptr<FormulaState> fs,
    std::set<std::pair<unsigned int, unsigned int>> unit_clauses) {
  bool found_conflict = false;

  std::stack<std::pair<unsigned int, unsigned int>> working;
  for (auto &t : unit_clauses)
    working.push(t);

  while (working.size() != 0) {
    unsigned int unit_clause, unit_prop;
    std::tie(unit_clause, unit_prop) = working.top();
    working.pop();

    if (!formula_.clauses_[unit_clause].is_unit(fs->a)) {
      // We found a conflict in this clause not previously caught by update_watched
      if (formula_.clauses_[unit_clause].eval(fs->a) == PropAssignment::False) {
        int backjump_level = learn_clause(fs, unit_clause, unit_prop);
        if (backjump_level >= 0) {
          backjump_ = backjump_level;
          should_backjump_ = true;
        }

        return true;
      }
      
      continue;
    } else {
      do_unit_preference(fs, unit_clause);
    }

    std::set<std::pair<unsigned int, unsigned int>> tmp_unit_clauses;
    std::pair<int, int> tmp_conflict_clause;
    std::tie(tmp_unit_clauses, tmp_conflict_clause) =
      update_watched(fs, unit_prop);

    if (tmp_conflict_clause.first >= 0) {
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

  return found_conflict;
}

void DPLLState::do_backjump() {
  assert(backjump_ >= 0);
  should_backjump_ = false;

  if (frontier_.size() > 0) {
    int num_jumped = 0;
    while (frontier_.top()->decision_level > backjump_ && frontier_.size() >= 1) {
      frontier_.pop();
      num_jumped += 1;
    }
  }

  backjump_ = -1;
}

std::pair<DPLLResult, Assignment> DPLLState::iterate() {
  Assignment empty;

  if (frontier_.size() == 0) {
    return {DPLLResult::Unsatisfiable, empty};
  }

  if (should_backjump_) {
    do_backjump();
    if (frontier_.size() == 0) {
      return {DPLLResult::Unsatisfiable, empty};
    } 
  }

  std::shared_ptr<FormulaState> current = frontier_.top();

  current->s = check_simplification_size(current->s);

  iterations_ += 1;
  total_iterations += 1;

  bool found_conflict = false;
  int prev_decision_prop = current->decision_prop;

  if (prev_decision_prop != -1) {
    std::set<std::pair<unsigned int, unsigned int>> unit_clauses;
    std::pair<int, int> conflict_clause;
    std::tie(unit_clauses, conflict_clause) = update_watched(current, prev_decision_prop);

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

  auto e = formula_.eval(current->a, current->s);
  if (e == PropAssignment::True) {
    return {DPLLResult::Satisfiable, current->a}; 
  } else if (e == PropAssignment::False) {
    while (!frontier_.top()->level_open[frontier_.top()->decision_level] && (frontier_.size() > 1)) {
      // If the current level is not open, pop the level above as well
      frontier_.pop();
    }
    frontier_.pop();
    return {DPLLResult::Unsatisfiable, empty};
  } 

  if (should_restart_) {
    log_info("Restarting due to learned unit clause");
    restart();
    iterations_ = 0;
    should_restart_ = false;
    return std::make_pair(DPLLResult::Unknown, empty);
  }

  if (found_conflict) {
    while (!frontier_.top()->level_open[frontier_.top()->decision_level] && (frontier_.size() > 1)) {
      // If the current level is not open, pop the level above as well
      frontier_.pop();
    }
    frontier_.pop();

    return {DPLLResult::Unsatisfiable, empty};
  }

  // If we learn the empty clause, then this formula is unsatisfiable
  if (found_unsat_) {
    while (!frontier_.empty()) {
      frontier_.pop();
    }
    return {DPLLResult::Unsatisfiable, empty};
  }

  if (iterations_ > restart_iterations_) {
    log_info("Currently have", formula_.get_num_clauses(), "clauses");
    iterations_ = 0;

    //restart_iterations_ += 0.5 * restart_iterations_;
    //restart();
    return {DPLLResult::Unknown, empty};
  }

  if (!found_conflict) {
    if (total_iterations == 1) {
      frontier_.pop();
    }

    // If this returns false, we've exhausted all possible splits
    bool able_to_split = do_splitting_rule(current); 

    if (!able_to_split) {
      while (!frontier_.top()->level_open[frontier_.top()->decision_level] && (frontier_.size() > 1)) {
        // If the current level is not open, pop the level above as well
        frontier_.pop();
      }
      frontier_.pop();
    }
  }
   
  return {DPLLResult::Unknown, empty};
}


// Free Functions
std::tuple<DPLLResult, Assignment, int> cannon::logic::dpll(CNFFormula f,
    PropFunc ph_func, AssignFunc ah_func, const std::chrono::seconds cutoff) {

  if (f.get_num_props() == 0) {
    std::valarray<PropAssignment> empty = {};
    return {DPLLResult::Satisfiable, empty, 0};
  }

  auto state = std::make_shared<DPLLState>(f, PropositionHeuristic(ph_func),
      AssignmentHeuristic(ah_func));

  int calls = 0;
  auto start_time = std::chrono::steady_clock::now();
  while (true) {
    DPLLResult r;
    Assignment a;
    calls += 1;

    std::tie(r, a) = state->iterate();
    if (r == DPLLResult::Satisfiable) {
      return std::make_tuple(r, a, calls);
    } else if (r == DPLLResult::Unsatisfiable && state->frontier_.size() == 0) {
      std::cout << "UNSAT" << std::endl;
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

std::tuple<DPLLResult, Assignment, int> cannon::logic::dpll(CNFFormula f, const
    std::chrono::seconds cutoff) {
  return dpll(f, default_prop, default_assign, cutoff);
}

// Default heuristics
unsigned int cannon::logic::default_prop(const DPLLState& ds, const Assignment&
    a, const Simplification& s, std::vector<unsigned int>& props) {
  RandomComparator rc(ds);
  VsidsComparator vc(ds, rc);
  TwoClauseComparator tc(ds, a, s, props, vc);

  std::sort(props.begin(), props.end(), tc);
  return props[props.size() - 1];
}

bool cannon::logic::default_assign(const CNFFormula& form, const Assignment& /*a*/, const
    Simplification& s, unsigned int prop, const std::vector<std::vector<unsigned int>>& watched) {
  std::random_device rd;
  static std::default_random_engine gen(rd());
  std::uniform_real_distribution<double> d;

  int num_pos = 0;
  int num_neg = 0;
  for (unsigned int c_num : watched[prop]) {
    if (s[c_num])
      continue;

    for (auto &l : form.clauses_[c_num].literals_) {
      if (l.prop_ == prop && l.negated_)
        num_neg += 1;

      if (l.prop_ == prop && !l.negated_)
        num_pos += 1;
    }
  }

  if (num_pos == num_neg) {
    double t = d(gen);
    return t < 0.5;
  } else {
    return num_pos < num_neg;
  }
}
