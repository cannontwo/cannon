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

        template <typename T>
        void log(const Level& level, const T& t) {
          for (auto l : loggers_) {
            l->log_internal_(level, level);
            l->log_internal_(level, t);
            l->log_internal_(level, "\n");
          }
        }

        template <typename... Targs>
        void log(const Level& level, const Targs&... targs) {
          for (auto l : loggers_) {
            l->log_internal_(level, level);
          }

          log_internal_(level, targs...);

          for (auto l : loggers_) {
            l->log_internal_(level, "\n");
          }
        }

      private:
        template <typename T>
        void log_internal_(const Level& level, const T& t) {
          for (auto l : loggers_) {
            l->log_internal_(level, t);
          }
        }

        template <typename T, typename... Targs>
        void log_internal_(const Level& level, const T& t, const Targs&... targs) {
          for (auto l : loggers_) {
            l->log_internal_(level, t);
            l->log_internal_(level, " ");
          }
          log_internal_(level, targs...);
        }


        std::vector<std::shared_ptr<Logger>> loggers_;
    };

    // Free functions
    void add_logger(std::shared_ptr<Logger> l);
    void add_logger(std::ostream &os, const Level &level = Level::info);
    
    template <typename T>
    void log(const Level& level, const T& t) {
      Registry::instance().log(level, t);
    }

    template <typename... Targs>
    void log(const Level& level, const Targs&... targs) {
      Registry::instance().log(level, targs...);
    }

    template <typename T>
    void log_info(const T &t) {
      log(Level::info, t);
    }

    template <typename... Targs>
    void log_info(const Targs&... targs) {
      log(Level::info, targs...);
    }

    template <typename T>
    void log_warning(const T &t) {
      log(Level::warning, t);
    }

    template <typename... Targs>
    void log_warning(const Targs&... targs) {
      log(Level::warning, targs...);
    }

    template <typename T>
    void log_error(const T &t) {
      log(Level::error, t);
    }

    template <typename... Targs>
    void log_error(const Targs&... targs) {
      log(Level::error, targs...);
    }

  } // namespace log
} // namespace cannon

#endif /* ifndef CANNON_LOG_REGISTRY_H */
