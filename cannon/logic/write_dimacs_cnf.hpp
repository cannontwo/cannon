#ifndef CANNON_LOGIC_WRITE_DIMACS_CNF_H_
#define CANNON_LOGIC_WRITE_DIMACS_CNF_H_ 

/*!
 * \file cannon/logic/write_dimacs_cnf.hpp
 * \brief File containing utilities for writing a CNF formula to DIMACS format.
 */

#include <string>

#include <cannon/utils/class_forward.hpp>

namespace cannon {
  namespace logic {

    CANNON_CLASS_FORWARD(CNFFormula);

    /*!
     * \brief Write a CNF formula to a DIMACS-formatted string.
     *
     * \param f The formula to write to string.
     *
     * \returns The DIMACS string.
     */
    std::string write_cnf(const CNFFormula& f);

  } // namespace logic
} // namespace cannon

#endif /* ifndef CANNON_LOGIC_WRITE_DIMACS_CNF_H_ */
