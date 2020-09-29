#ifndef CANNON_LOG_LOGGER_H
#define CANNON_LOG_LOGGER_H 

#include <iostream>
#include <memory>
#include <string>

#include <cannon/log/level.hpp>

namespace cannon {
  namespace log {

    class Logger {
      public:
        Logger(std::ostream& os, const Level& l = Level::info) : os_(os), level_(l) {}

        void log(const std::string& s, const Level& l = Level::info);

        void set_level(const Level& l);

      private:
        std::ostream& os_;
        Level level_;
    };

  } // namespace log
} // namespace cannon

#endif /* ifndef CANNON_LOG_LOGGER_H */
