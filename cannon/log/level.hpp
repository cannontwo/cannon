#ifndef CANNON_LOG_LEVEL_H
#define CANNON_LOG_LEVEL_H 

#include <iostream>

namespace cannon {
  namespace log {

    enum Level {
      info,
      warning,
      error
    };

    std::ostream& operator<<(std::ostream& os, const Level& l); 
  } // namespace log
} // namespace cannon


#endif /* ifndef CANNON_LOG_LEVEL_H */
