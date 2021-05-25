#ifndef CANNON_LOG_LEVEL_H
#define CANNON_LOG_LEVEL_H 

#include <iostream>

/*!
 * \file cannon/log/level.hpp
 * \brief File containing Level enumeration.
 */

/*!
 * \namespace cannon::log
 * \brief Namespace containing logging utilities for writing information to
 * arbitrary files and streams.
 */

namespace cannon {
  namespace log {

    /*!
     * \brief Enumeration representing the level of severity of a particular
     * Logger or message.
     */
    enum Level {
      info,
      warning,
      error
    };

    std::ostream& operator<<(std::ostream& os, const Level& l); 
  } // namespace log
} // namespace cannon


#endif /* ifndef CANNON_LOG_LEVEL_H */
