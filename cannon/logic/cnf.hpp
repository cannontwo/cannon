#ifndef CANNON_LOGIC_CNF_H
#define CANNON_LOGIC_CNF_H 

/*!
 * \file cannon/logic/cnf.hpp
 * \brief File containing classes and utility functions for working with
 * Conjunctive Normal Form (CNF) formulas.
 */

#include <set>
#include <vector>
#include <iostream>
#include <valarray>
#include <algorithm>
#include <iostream>

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace logic {

    /*!
     * \brief Enumeration representing the three possible states that can be
     * assigned to a proposition. 
     */ 
    enum class PropAssignment {
      False,
      Unassigned,
      True
    };

    using Assignment=std::valarray<PropAssignment>;
    using Simplification=std::valarray<bool>;

    /*!
     * \brief Class representing a single literal in a CNF formula.
     */
    class Literal {
      public:
        Literal() = delete;

        /*!
         * \brief Constructor taking a proposition number for this literal and
         * whether it is negated.
         */
        Literal(unsigned int prop_num, bool negated) : prop_(prop_num), negated_(negated) {}

        /*!
         * \brief Copy constructor.
         */
        Literal(const Literal& l) : prop_(l.prop_), negated_(l.negated_) {}
        
        /*!
         * \brief Move constructor.
         */
        Literal(Literal&& l) : prop_(l.prop_), negated_(l.negated_) {}

        /*!
         * \brief Copy assignment operator.
         */
        Literal& operator=(const Literal &o) {
          prop_ = o.prop_;
          negated_ = o.negated_;
          return *this;
        }

        /*!
         * \brief Evaluate this literal with the input assignment. The
         * proposition number stored by this literal is taken as an index into
         * the assignment.
         *
         * \param assignment An assignment of values to propositions to
         * evaluate this literal on.
         *
         * \returns The value of this literal in the assignment.
         */
        PropAssignment eval(const Assignment& assignment) const;

        /*!
         * \brief Comparison operator for sorting literals.
         */
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

        unsigned int prop_; //!< Proposition number for this literal
        bool negated_; //!< Whether this literal is negated
    };

    /*!
     * \brief Class representing a clause in a Conjunctive Normal Form (CNF)
     * formula, which is defined as a disjunction of literals.
     */
    class Clause {
      public:
        
        /*!
         * \brief Default constructor.
         */
        Clause() {}

        /*!
         * \brief Copy constructor.
         */
        Clause(const Clause& o) : literals_(o.literals_), num_props_(o.num_props_) {}
        
        /*!
         * \brief Move constructor.
         */
        Clause(Clause&& o) : literals_(std::move(o.literals_)), num_props_(o.num_props_) {}

        /*!
         * \brief Destructor.
         */
        ~Clause() {}

        /*!
         * \brief Copy assignment operator.
         */
        Clause& operator=(const Clause& o) {
          literals_ = o.literals_;
          num_props_ = o.num_props_;
          return *this;
        }

        /*!
         * \brief Add a literal to this clause.
         *
         * \param l The literal to add to this clause.
         */
        void add_literal(Literal l);
        
        /*!
         * \brief Construct and add a literal to this clause.
         *
         * \param prop_num Proposition number for the clause to add.
         * \param negated Whether the literal should be negated.
         */
        void add_literal(unsigned int prop_num, bool negated);

        /*!
         * \brief Get the number of literals in this clause.
         *
         * \returns Clause size.
         */
        unsigned int size() const;

        /*!
         * \brief Get the number of unassigned literals in this clause with
         * respect to the input assignment.
         *
         * \param a Assignment to use to compute size.
         *
         * \returns The number of unassigned literals in this clause.
         */
        unsigned int size(const Assignment& a) const;

        /*!
         * \brief Determine if this clause is a unit clause.
         *
         * \returns Whether this clause has only a single literal.
         */
        bool is_unit() const;

        /*!
         * \brief Determine if this clause is a unit clause with respect to the
         * input assignment.
         *
         * \param a Assignment to use to compute if this clause is a unit.
         *
         * \returns Whether this clause has only a single unassigned literal.
         */
        bool is_unit(const Assignment& a) const;

        /*!
         * \brief Get the propositions in this clause which are unassigned in
         * the input assignment.
         *
         * \param a Assignment to compute props with respect to.
         *
         * \returns Set of propositions in this clause.
         */
        std::set<unsigned int> get_props(const Assignment& a);

        /*!
         * \brief Determine whether this clause includes the input prop when
         * simplified according to the input assignment.
         *
         * \param a The assignment to check membership with respect to.
         * \param prop The prop to check for membership.
         *
         * \returns Whether this clause contains the query prop.
         */
        bool contains_prop(const Assignment& a, unsigned int prop) const;

        /*!
         * \brief Determine whether this clause includes an unnegated literal
         * of the input prop when simplified according to the input assignment.
         *
         * \param a The assignment to check membership with respect to.
         * \param prop The prop to check for membership.
         *
         * \returns Whether this clause contains the query prop.
         */
        bool has_pos_literal(const Assignment& a, unsigned int prop) const;

        /*!
         * \brief Evalute this clause on the input assignment. Clauses evaluate
         * to True if a single Literal in them evaluates to True, and otherwise
         * evalute to Unassigned if any Literal evalues to Unassigned. A Clause
         * evaluates to False iff all contained Literals evaluate to False.
         * Trivially, an empty Clause evaluates to False.
         *
         * \param a Assignment to evaluate this clause on.
         *
         * \returns Evaluation of this clause.
         */
        PropAssignment eval(const Assignment& assignment) const;

        /*!
         * \brief If this clause is a unit with respect to the input
         * assignment, get the corresponding proposition.
         *
         * \param a Assignment that this clause is a unit with respect to.
         *
         * \returns Proposition for unit clause.
         */
        unsigned int get_unit_prop(const Assignment& a);

        /*!
         * \brief If this clause is a unit with respect to the input
         * assignment, get whether it is negated.
         *
         * \param a Assignment that this clause is a unit with respect to.
         *
         * \returns Whether the unit clause is negated.
         */
        bool get_unit_negated(const Assignment& a);

        /*!
         * \brief Get the assignment for the input proposition which causes
         * this clause to evaluate to True.
         *
         * \param prop Proposition to get assignment for.
         *
         * \returns Assignment for input prop that causes this clause to
         * evaluate to True, or Unassigned if the proposition does not appear
         * in this clause.
         */
        PropAssignment get_assignment_for_literal(unsigned int prop);

        friend std::ostream& operator<<(std::ostream& os, const Clause& c);

        friend class CNFFormula;

        bool operator==(const Clause& c) const {
          if (c.literals_.size() == literals_.size()) {
            for (auto& l : literals_) {
              if (c.literals_.find(l) == c.literals_.end()) {
                return false;
              }
            }

            return true;
          } else {
            return false;
          }
        }

        std::set<Literal> literals_; //!< Literals in this clause

      private:
        unsigned int num_props_ = 0; //!< Number of propositions implied by proposition numbers stored in this Clause
    };

    /*!
     * \brief Class representing a Conjunctive Normal Form formula, which is a
     * conjunction of Clauses.
     */
    class CNFFormula {
      public:

        /*!
         * \brief Add a Clause to this formula. 
         *
         * \param c Clause to add. This argument is an rval to avoid copying Literal set.
         */
        void add_clause(Clause&& c);

        /*!
         * \brief Construct and add a unit clause to this formula.
         *
         * \param prop Proposition for the unit clause.
         * \param negated Whether the proposition should be negated.
         */
        void add_unit_clause(unsigned int prop, bool negated);

        /*!
         * \brief Evaluate this formula on the input assignment and
         * simplification. Clauses are simplified when their truth can be
         * deduced from a partial assignment, and so further proposition
         * assignments cannot change their truth value.
         *
         * \param assignment Assignment to evaluate this formula on.
         * \param s Simplification to accelerate evaluation.
         */
        PropAssignment eval(const Assignment& assignment,
            const Simplification& s) const;

        /*!
         * \brief Get propositions occuring in unit clauses in this formula.
         *
         * \param a Assignment to find unit clauses with respect to.
         * \param s Simplification to accelerate unit clause finding.
         *
         * \returns Vector of tuples containing propositions that occur in unit
         * clauses, whether they are negated, and indices of the clauses in which
         * they occur.
         */
        std::vector<std::tuple<unsigned int, bool, int>> get_unit_clause_props(const
            Assignment& a, const Simplification& s) const;

        /*!
         * \brief Get propositions occuring in unit clauses in this formula.
         *
         * \param a Assignment to find unit clauses with respect to.
         * \param s Simplification to accelerate unit clause finding.
         * \param watched Watched list for all propositions used to speed up search.
         *
         * \returns Vector of tuples containing propositions that occur in unit
         * clauses, whether they are negated, and indices of the clauses in which
         * they occur.
         */
        std::vector<std::tuple<unsigned int, bool, int>> get_unit_clause_props(const
            Assignment& a, const Simplification& s,
            std::vector<std::vector<unsigned int>> watched) const;

        /*!
         * \brief Get propositions occuring in unit clauses in this formula.
         *
         * \param a Assignment to find unit clauses with respect to.
         * \param s Simplification to accelerate unit clause finding.
         * \param unit_clauses Candidate clauses to check for being units.
         *
         * \returns Vector of tuples containing propositions that occur in unit
         * clauses, whether they are negated, and indices of the clauses in which
         * they occur.
         */
        std::vector<std::tuple<unsigned int, bool, int>> get_unit_clause_props(const
            Assignment& a, const Simplification& s, std::vector<unsigned int>
            unit_clauses) const;

        /*!
         * \brief Get the size of the smallest clause in this Formula.
         *
         * \param a Assignment to compute clause sizes with respect to.
         * \param s Simplification to speed up checking.
         *
         * \returns Smallest clause size.
         */
        unsigned int get_smallest_clause_size(const Assignment& a, const Simplification& s) const;

        /*!
         * \brief Get the number of propositions in this formula.
         *
         * \returns Number of propositions.
         */
        unsigned int get_num_props() const;
        
        /*!
         * \brief Get the number of clauses in this formula.
         *
         * \returns The number of clauses.
         */
        unsigned int get_num_clauses() const;

        /*!
         * \brief Get the number of clauses containing the input proposition
         * and one other literal in this formula.
         *
         * \param a Assignment to compute clauses sizes with respect to.
         * \param s Simplification to speed counting.
         * \param prop Proposition to compute two-clauses for.
         *
         * \returns Number of two-clauses in this formula for the input proposition.
         */
        unsigned int get_num_two_clauses(const Assignment& a, const
            Simplification& s, unsigned int prop) const;

        /*!
         * \brief Get the number of two-clauses for each proposition in the input.
         *
         * \param a Assignment to compute clauses sizes with respect to.
         * \param s Simplification to speed counting.
         * \param props Propositions to compute two-clauses for.
         *
         * \returns Number of two-clauses in this formula for each input proposition.
         */
        std::vector<unsigned int> get_num_two_clauses(const Assignment& a, const
            Simplification& s, const std::vector<unsigned int>& props) const;

        /*!
         * \brief Get all unassigned propositions in this formula.
         *
         * \param a Assignment to compute unassigned propositions with respect to.
         * \param s Simplification to speed up search.
         *
         * \returns Unassigned propositions.
         */
        std::vector<unsigned int> get_props(const Assignment& a,
            const Simplification& s);

        /*!
         * \brief Make adjacency matrix for this formula. Two propositions are
         * adjacent if they occur in the same clause.
         *
         * \param a Assignment to compute adjacency matrix with respect to.
         * \param s Simplification to speed up construction.
         *
         * \returns Adjacency matrix
         */
        MatrixXd make_adjacency_mat(const Assignment& a, const Simplification& s) const;

        /*!
         * \brief Get multiset of all unassigned propositions in this formula.
         *
         * \param a Assignment to compute unassigned propositions with respect to.
         * \param s Simplification to speed up search.
         *
         * \returns Unassigned propositions.
         */
        std::multiset<unsigned int> get_props_multiset(const Assignment& a,
            const Simplification& s);

        /*!
         * \brief Check whether the input proposition is a pure literal in this
         * formula. A pure literal is one that appears only as a positive
         * proposition or negated proposition in the formula.
         *
         * \param a Assignment to compute whether the input prop is a pure
         * literal with respect to.
         * \param s Simplification to speed up search.
         * \param prop Proposition to check.
         *
         * \returns Unassigned if the input prop is not a pure literal in this
         * formula, or else the assignment (True or False) which makes literals
         * containing the input prop evaluate true.
         */
        PropAssignment is_pure_literal(const Assignment& a, const Simplification& s,
            unsigned int prop);

        /*!
         * \brief Generate a simplification for this formula.
         *
         * \param a Assignment to generate a simplification with respect to.
         * \param s Previous simplification to speed up generation.
         *
         * \returns A new simplification skipping clauses which evaluate to true.
         */
        Simplification simplify(const Assignment& a,
            const Simplification& s) const;

        /*!
         * \brief Merge the input formula into this formula.
         *
         * \param f Formula to merge.
         */
        void merge(CNFFormula&& f);

        bool operator==(const CNFFormula f) const {
          if (f.clauses_.size() == clauses_.size()) {
            for (auto& c : clauses_) {
              if (std::find(f.clauses_.begin(), f.clauses_.end(), c) == f.clauses_.end()) {
                return false;
              }
            }

            return true;
          } else {
            return false;
          }
        }

        friend std::ostream& operator<<(std::ostream& os, const CNFFormula& f);

        std::vector<Clause> clauses_; //!< Clauses in this formula

      private:
        unsigned int num_props_ = 0; //!< Number of propositions in this formula
    };

    /*!
     * \brief Generate a random 3-clause using propositions 0 to num_props-1.
     *
     * \param num_props Maximum number of propositions to choose from.
     *
     * \returns Generated clause.
     */
    Clause generate_random_clause(unsigned int num_props);

    /*!
     * \brief Generate a random 3-CNF formula.
     *
     * \param num_props Maximum number of propositions to choose from.
     * \param num_clauses Number of clauses in random formula
     *
     * \returns Generated formula.
     */
    CNFFormula generate_random_formula(unsigned int num_props, unsigned int num_clauses);

    /*!
     * \brief Attempt to resolve two CNF clauses on the input proposition.
     * Throws an exception if the two clauses cannot be resolved on the input
     * prop (i.e., they do not both contain the proposition or the literals
     * containing the proposition don't have opposite negation).
     *
     * \param c1 First clause to resolve, which will be modified to contain the resolved clause
     * \param c2 Second clause to use in resolution.
     * \param prop Proposition to resolve on.
     */
    void resolve(Clause& c1, const Clause& c2, unsigned int prop);

    std::ostream& operator<<(std::ostream& os, PropAssignment a);
    std::ostream& operator<<(std::ostream& os, const Literal& l);
    std::ostream& operator<<(std::ostream& os, const Clause& c);
    std::ostream& operator<<(std::ostream& os, const CNFFormula& f);

    std::ostream& operator<<(std::ostream& os, const std::valarray<PropAssignment>& v);
    bool operator==(const std::valarray<PropAssignment>& v1, const std::valarray<PropAssignment>& v2);
    bool operator!=(const std::valarray<PropAssignment>& v1, const std::valarray<PropAssignment>& v2);

  } // namespace logic
} // namespace cannon

#endif /* ifndef CANNON_LOGIC_CNF_H */
