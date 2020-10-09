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

        template <typename T>
        void log(const Level& l, const T& t) {
          if (l >= level_) {
            os_ << l << t << std::endl;
          }
        }

        void set_level(const Level& l);

        friend class Registry;

      private:
         
        template <typename T>
        void log_internal_(const Level& l, const T& t) {
          if (l >= level_) {
            os_ << t;
          }
        }

        template <typename T, typename... Targs>
        void log_internal_(const Level& l, const T& t, const Targs&... targs) {
          if (l >= level_) {
            os_ << t;
          }
          log_internal_(l, targs...);
        }

        std::ostream& os_;
        Level level_;
    };

  } // namespace log
} // namespace cannon

#endif /* ifndef CANNON_LOG_LOGGER_H */
