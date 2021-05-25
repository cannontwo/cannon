#ifndef CANNON_LOG_LOGGER_H
#define CANNON_LOG_LOGGER_H 

/*!
 * \file cannon/log/logger.hpp
 * \brief File containing Logger class definition.
 */

#include <iostream>
#include <memory>
#include <string>

#include <cannon/log/level.hpp>

namespace cannon {
  namespace log {

    /*!
     * \brief Class representing an encapsulated stream for writing log
     * messages with a certain level of severity. A Logger will print all
     * levels with equal or greater severity than the Level that it is
     * initialized with.
     */
    class Logger {
      public:

        /*!
         * \brief Constructor taking a stream and level for printing.
         *
         * \param os The output stream to encapsulate.
         * \param l The level of this logger.
         */
        Logger(std::ostream& os, const Level& l = Level::info) : os_(os), level_(l) {}

        /*!
         * \brief Log a message with an associated level according to this
         * Logger's internal level.
         *
         * \param l The level of the message to be logged.
         * \param t The message to log, which is any type implementing operator<<(std::ostream&)
         */
        template <typename T>
        void log(const Level& l, const T& t) {
          if (l >= level_) {
            os_ << l << t << std::endl;
          }
        }

        /*!
         * \brief Set the logging level of this object.
         *
         * \param l The new level.
         */
        void set_level(const Level& l);

        friend class Registry;

      private:
         
        /*!
         * \brief Base case of recursive logging function.
         *
         * \param l The level of the message.
         * \param t The message to log, which is any type implementing operator<<(std::ostream&)
         */
        template <typename T>
        void log_internal_(const Level& l, const T& t) {
          if (l >= level_) {
            os_ << t;
          }
        }

        /*!
         * \brief Vararg version of the logging function, templated so that
         * each argument can have a different type.
         *
         * \param l The level to log the input message with.
         * \param t Current parameter under consideration.
         * \param targs Remaining varargs parameters to unpack recursively.
         */
        template <typename T, typename... Targs>
        void log_internal_(const Level& l, const T& t, const Targs&... targs) {
          if (l >= level_) {
            os_ << t;
          }
          log_internal_(l, targs...);
        }

        std::ostream& os_; //!< Output stream managed by this object.
        Level level_; //!< Log level of this object.
    };

  } // namespace log
} // namespace cannon

#endif /* ifndef CANNON_LOG_LOGGER_H */
