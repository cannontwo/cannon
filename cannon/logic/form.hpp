#ifndef CANNON_LOGIC_FORM_H
#define CANNON_LOGIC_FORM_H 

/*!
 * \file cannon/logic/form.hpp
 * \brief File containing utility classes for representing arbitrary boolean
 * formulas.
 */

#include <vector>
#include <memory>
#include <stack>

#include <cannon/utils/class_forward.hpp>

namespace cannon {
  namespace logic {

    CANNON_CLASS_FORWARD(CNFFormula);

    // Note below we use not short-circuiting 'or' and 'and' so that we throw
    // correctly if not enough truth values are provided to eval().
    
    /*!
     * \brief Abstract base class representing a boolean formula.
     */
    class Formula {
      public:
        
        /*!
         * \brief Evaluate this formula on the input boolean assignment.
         *
         * \param assignment Proposition assignments to evaluate formula on.
         *
         * \returns Value of formula under assignment.
         */
        virtual bool eval(const std::vector<bool>& assignment) const = 0;
        
        /*!
         * \brief Convert this formula to conjunctive normal form.
         *
         * \param negated Whether this formula is being negated, which affects
         * CNF resolution.
         *
         * \returns Equivalent CNF formula.
         */
        virtual CNFFormula to_cnf(bool negated = false) const = 0;

        /*!
         * \brief Destructor.
         */
        virtual ~Formula() {}
    };

    /*!
     * \brief Class representing an atomic proposition.
     */
    class Atomic : public Formula {
      public:

        /*!
         * \brief Constructor taking the index of the proposition represented
         * by this class.
         */
        Atomic(unsigned idx) : idx(idx) {}

        /*!
         * \brief Inherited from Formula.
         */
        virtual bool eval(const std::vector<bool>& assignment) const override;

        /*!
         * \brief Inherited from Formula.
         */
        virtual CNFFormula to_cnf(bool negated = false) const override;

        /*!
         * \brief Destructor.
         */
        virtual ~Atomic() {}

        const unsigned idx; //!< Proposition index for this atomic variable
    };

    /*!
     * \brief Class representing the negation of a contained formula.
     */
    class Negation : public Formula {
      public:

        /*!
         * \brief Constructor taking a formula to negate.
         */
        Negation(const Formula& formula) : formula(std::make_shared<Negation>(formula)) {}

        /*!
         * \brief Constructor taking a formula to negate.
         */
        Negation(std::shared_ptr<Formula> formula) : formula(formula) {}

        /*!
         * \brief Inherited from Formula.
         */
        virtual bool eval(const std::vector<bool>& assignment) const override;

        /*!
         * \brief Inherited from Formula.
         */
        virtual CNFFormula to_cnf(bool negated = false) const override;

        /*!
         * \brief Destructor.
         */
        virtual ~Negation() {}

        const std::shared_ptr<Formula> formula; //!< Formula negated by this class.
    };

    /*!
     * \brief Class representing the boolean AND operator.
     */
    class And : public Formula {
      public:
        
        /*!
         * \brief Constructor taking two formulas to be combined by the AND
         * operator.
         */
        And(std::shared_ptr<Formula> left, std::shared_ptr<Formula> right) :
          left(left), right(right) {}

        /*!
         * \brief Inherited from Formula.
         */
        virtual bool eval(const std::vector<bool>& assignment) const override;

        /*!
         * \brief Inherited from Formula.
         */
        virtual CNFFormula to_cnf(bool negated = false) const override;

        /*!
         * \brief Destructor.
         */
        virtual ~And() {}

        const std::shared_ptr<Formula> left; //!< First input to AND
        const std::shared_ptr<Formula> right; //!< Second input to AND
    };

    /*!
     * \brief Class representing a boolean OR operator.
     */
    class Or : public Formula {
      public:
        
        /*!
         * \brief Constructor taking two formulas to be combined by the OR
         * operator.
         */
        Or(std::shared_ptr<Formula> left, std::shared_ptr<Formula> right) :
          left(left), right(right) {}

        /*!
         * \brief Inherited from Formula.
         */
        virtual bool eval(const std::vector<bool>& assignment) const override;

        /*!
         * \brief Inherited from Formula.
         */
        virtual CNFFormula to_cnf(bool negated = false) const override;

        /*!
         * \brief Destructor.
         */
        virtual ~Or() {}

        const std::shared_ptr<Formula> left; //!< First input to OR
        const std::shared_ptr<Formula> right; //!< Second input to OR
    };

    /*!
     * \brief Class representing a boolean implication operator.
     */
    class Implies : public Formula {
      public:

        /*!
         * \brief Constructor taking two formulas to be combined by implication.
         */
        Implies(std::shared_ptr<Formula> left, std::shared_ptr<Formula> right) :
          left(left), right(right) {}

        /*!
         * \brief Inherited from Formula.
         */
        virtual bool eval(const std::vector<bool>& assignment) const override;

        /*!
         * \brief Inherited from Formula.
         */
        virtual CNFFormula to_cnf(bool negated = false) const override;

        /*!
         * \brief Destructor.
         */
        virtual ~Implies() {}

        const std::shared_ptr<Formula> left; //!< Left-hand input to implication
        const std::shared_ptr<Formula> right; //!< Right-hand input to implication
    };

    /*!
     * \brief Class representing a boolean if-and-only-if operator.
     */
    class Iff : public Formula {
      public:

        /*!
         * \brief Constructor taking two formulas to be combined by if-and-only-if.
         */
        Iff(std::shared_ptr<Formula> left, std::shared_ptr<Formula> right) :
          left(left), right(right) {}

        /*!
         * \brief Inherited from Formula.
         */
        virtual bool eval(const std::vector<bool>& assignment) const override;

        /*!
         * \brief Inherited from Formula.
         */
        virtual CNFFormula to_cnf(bool negated = false) const override;

        /*!
         * \brief Destructor.
         */
        virtual ~Iff() {}

        const std::shared_ptr<Formula> left; //!< First input to iff
        const std::shared_ptr<Formula> right; //!< Second input to iff
    };

    // Free functions
    
    /*!
     * \brief Create a negation of the input formula.
     *
     * \param f The formula to negate.
     *
     * \returns Created negation.
     */
    std::shared_ptr<Negation> make_negation(std::shared_ptr<Formula> f);

    /*!
     * \brief Create an AND of the two input formulas.
     *
     * \param f First input formula.
     * \param g Second input formula.
     *
     * \returns Created AND operator.
     */
    std::shared_ptr<And> make_and(std::shared_ptr<Formula> f, std::shared_ptr<Formula> g);

    /*!
     * \brief Create an AND of an arbitrary number of input formulas.
     *
     * \param f First input formula.
     * \param fs Stack of input formulas to be chained by AND.
     *
     * \returns Top-level created AND operator.
     */
    std::shared_ptr<And> make_and(std::shared_ptr<Formula> f, std::stack<std::shared_ptr<Formula>>& fs);
    
    /*!
     * \brief Create an OR of the two input formulas.
     *
     * \param f First input formula.
     * \param g Second input formula.
     *
     * \returns Created OR operator.
     */
    std::shared_ptr<Or> make_or(std::shared_ptr<Formula> f, std::shared_ptr<Formula> g);

    /*!
     * \brief Create an OR of an arbitrary number of input formulas.
     *
     * \param f First input formula.
     * \param fs Stack of input formulas to be chained by OR.
     *
     * \returns Top-level created OR operator.
     */
    std::shared_ptr<Or> make_or(std::shared_ptr<Formula> f, std::stack<std::shared_ptr<Formula>>& fs);

    /*!
     * \brief Create an implication operator for the input formulas.
     *
     * \param f The left-hand formula in the implication.
     * \param g The right-hand formula in the implication.
     *
     * \returns Created implication operator.
     */
    std::shared_ptr<Implies> make_implies(std::shared_ptr<Formula> f, std::shared_ptr<Formula> g);

    /*!
     * \brief Create an IFF operator for the input formulas.
     *
     * \param f First input formula.
     * \param g Second input formula.
     * 
     * \returns The created IFF operator.
     */
    std::shared_ptr<Iff> make_iff(std::shared_ptr<Formula> f, std::shared_ptr<Formula> g);

  } // namespace logic
} // namespace cannon

#endif /* ifndef CANNON_LOGIC_FORM_H */
