#ifndef CANNON_LOG_REGISTRY_H
#define CANNON_LOG_REGISTRY_H 

#include <vector>
#include <memory>
#include <string>
#include <iostream>

#include <cannon/log/logger.hpp>

namespace cannon {
  namespace log {

    // Singleton registry. Taking inspiration from
    // https://github.com/gabime/spdlog/blob/master/include/spdlog/details/registry.h
    class Registry {
      public:
        Registry() {
          add_logger(std::make_shared<Logger>(std::cout));
        }

        static Registry& instance() {
          static Registry static_instance;

          return static_instance;
        }

        void add_logger(std::shared_ptr<Logger> l);


        // TODO Make variadic
        template <typename T>
        void log(const T& t, const Level& level = Level::info) {
          for (auto l : loggers_) {
            l->log(t, level);
          }
        }

      private:
        std::vector<std::shared_ptr<Logger>> loggers_;
    };

    // Free functions
    void add_logger(std::shared_ptr<Logger> l);
    void add_logger(std::ostream &os, const Level &level = Level::info);
    
    template <typename T>
    void log(const T& t, const Level& level = Level::info) {
      Registry::instance().log(t, level);
    }

    template <typename T>
    void log_info(const T &t) {
      log(t, Level::info);
    }

    template <typename T>
    void log_warning(const T &t) {
      log(t, Level::warning);
    }

    template <typename T>
    void log_error(const T &t) {
      log(t, Level::error);
    }

  } // namespace log
} // namespace cannon

#endif /* ifndef CANNON_LOG_REGISTRY_H */
