#ifndef CANNON_LOGIC_READ_DIMACS_CNF_H
#define CANNON_LOGIC_READ_DIMACS_CNF_H 

/*!
 * \file cannon/logic/read_dimacs_cnf.hpp
 * \brief File containing utilities for parsing formulas from DIMACS files.
 */

#include <string>

#include <cannon/utils/class_forward.hpp>

namespace cannon {
  namespace logic {

    CANNON_CLASS_FORWARD(CNFFormula);
    CANNON_CLASS_FORWARD(Clause);

    /*!
     * \brief Parse a conjunctive normal form formula from the input DIMACS
     * string. See https://people.sc.fsu.edu/~jburkardt/data/cnf/cnf.html
     *
     * \param s The string to parse
     *
     * \returns The parsed CNF formula.
     */
    CNFFormula parse_cnf(const std::string& s);

    /*!
     * \brief Parse a CNF formula from the input DIMACS-formatted file.
     *
     * \param path The file path to read.
     *
     * \returns The parsed CNF formula.
     */
    CNFFormula load_cnf(const std::string& path);

    /*!
     * \brief Parse a single clause from the input DIMACS string.
     *
     * \param s String to parse a clause from. Will be modified.
     *
     * \returns The parsed clause.
     */
    Clause parse_clause(std::string& s);
    
    /*!
     * \brief Parse a single clause from the input DIMACS string.
     *
     * \param s String to parse a clause from. 
     *
     * \returns The parsed clause.
     */
    Clause parse_clause(const std::string& s);

  } // namespace logic
} // namespace cannon
#endif /* ifndef CANNON_LOGIC_READ_DIMACS_CNF_H */
