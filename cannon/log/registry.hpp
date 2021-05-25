#ifndef CANNON_LOG_REGISTRY_H
#define CANNON_LOG_REGISTRY_H 

/*!
 * \file cannon/log/registry.hpp
 * \brief File containing Registry class definition.
 */

#include <vector>
#include <memory>
#include <string>
#include <iostream>

#include <cannon/log/logger.hpp>

namespace cannon {
  namespace log {

    /*!
     * \brief Singleton logger registry class which manages all logging behavior.
     *
     * Taking inspiration from
     * https://github.com/gabime/spdlog/blob/master/include/spdlog/details/registry.h
     */
    class Registry {
      public:

        /*!
         * \brief Default constructor. Constructs a logger for std::cout.
         */
        Registry() {
          add_logger(std::make_shared<Logger>(std::cout));
        }

        /*!
         * \brief Retrieve the static singleton instance of the registry.
         *
         * \returns The singleton registry.
         */
        static Registry& instance() {
          static Registry static_instance;

          return static_instance;
        }

        /*!
         * \brief Add a Logger to this registry.
         *
         * \param l The Logger to add.
         */
        void add_logger(std::shared_ptr<Logger> l);

        /*!
         * \brief Log a message with a certain severity by passing it to each
         * registered Logger.
         *
         * \param level The severity level of the message.
         * \param t The message, which is templated. \sa cannon::log::Logger.
         */
        template <typename T>
        void log(const Level& level, const T& t) {
          for (auto l : loggers_) {
            l->log_internal_(level, level);
            l->log_internal_(level, t);
            l->log_internal_(level, "\n");
          }
        }

        /*!
         * \brief Log a message consisting of a variable number of arguments to
         * each registered Logger.
         *
         * \param level The severity level of the message.
         * \param targs Vararg container for all parameters.
         */
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
        
        /*!
         * \brief Base case of recursive logging function.
         *
         * \param level Severity level of message to log.
         * \param t Message to log.
         */
        template <typename T>
        void log_internal_(const Level& level, const T& t) {
          for (auto l : loggers_) {
            l->log_internal_(level, t);
          }
        }

        /*!
         * \brief Recursive logging function for handling vararg messages.
         *
         * \param level Severity level of message to log.
         * \param t Single message, templated to be printable.
         * \param targs Remaining varargs parameters to unpack.
         */
        template <typename T, typename... Targs>
        void log_internal_(const Level& level, const T& t, const Targs&... targs) {
          for (auto l : loggers_) {
            l->log_internal_(level, t);
            l->log_internal_(level, " ");
          }
          log_internal_(level, targs...);
        }

        std::vector<std::shared_ptr<Logger>> loggers_; //!< Loggers registered to this registry
    };

    // Free functions
    
    /*!
     * \brief Add a logger to the singleton logging registry.
     *
     * \param l The logger to add.
     */
    void add_logger(std::shared_ptr<Logger> l);

    /*!
     * \brief Create a logger from an output stream and severity level and add
     * it to the singleton loggin registry.
     *
     * \param os The output stream to log to.
     * \param level The severity level for this new logger.
     */
    void add_logger(std::ostream &os, const Level &level = Level::info);
    
    /*!
     * \brief Log a message with a given level.
     *
     * \param level The message severity level.
     * \param t The message to log.
     */
    template <typename T>
    void log(const Level& level, const T& t) {
      Registry::instance().log(level, t);
    }

    /*!
     * \brief Log a vararg message with a given level. 
     *
     * \param level Severity level for the message.
     * \param targs Vararg container to unpack. Each argument will be separated
     * by a space from the next argument in the output log.
     */
    template <typename... Targs>
    void log(const Level& level, const Targs&... targs) {
      Registry::instance().log(level, targs...);
    }

    /*!
     * \brief Log a message with the severity level Info.
     *
     * \param t The message to log.
     */
    template <typename T>
    void log_info(const T &t) {
      log(Level::info, t);
    }

    /*!
     * \brief Log a vararg message with the severity level Info.
     * 
     * \param targs Vararg container with message to log.
     */
    template <typename... Targs>
    void log_info(const Targs&... targs) {
      log(Level::info, targs...);
    }

    /*!
     * \brief Log a message with the severity level Warning.
     *
     * \param t The message to log.
     */
    template <typename T>
    void log_warning(const T &t) {
      log(Level::warning, t);
    }

    /*!
     * \brief Log a vararg message with the severity level Warning.
     * 
     * \param targs Vararg container with message to log.
     */
    template <typename... Targs>
    void log_warning(const Targs&... targs) {
      log(Level::warning, targs...);
    }

    /*!
     * \brief Log a message with the severity level Error.
     *
     * \param t The message to log.
     */
    template <typename T>
    void log_error(const T &t) {
      log(Level::error, t);
    }

    /*!
     * \brief Log a vararg message with the severity level Error.
     * 
     * \param targs Vararg container with message to log.
     */
    template <typename... Targs>
    void log_error(const Targs&... targs) {
      log(Level::error, targs...);
    }

  } // namespace log
} // namespace cannon

#endif /* ifndef CANNON_LOG_REGISTRY_H */
