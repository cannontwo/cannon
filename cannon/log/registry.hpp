#ifndef CANNON_LOG_REGISTRY_H
#define CANNON_LOG_REGISTRY_H 

#include <vector>
#include <memory>
#include <string>
#include <iostream>

#include <cannon/log/logger.hpp>

namespace cannon {
  namespace log {

    // Singleton registry
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

        void log(const std::string& s, const Level& level = Level::info);

      private:
        std::vector<std::shared_ptr<Logger>> loggers_;
    };

    // Free functions
    void add_logger(std::shared_ptr<Logger> l);
    void add_logger(std::ostream &os, const Level &level = Level::info);
    void log(const std::string &s, const Level& level = Level::info);
    void log_info(const std::string &s);
    void log_warning(const std::string &s);
    void log_error(const std::string &s);

  } // namespace log
} // namespace cannon

#endif /* ifndef CANNON_LOG_REGISTRY_H */
