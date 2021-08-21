#ifndef CANNON_UTILS_ARGPARSER_H
#define CANNON_UTILS_ARGPARSER_H 

/*!
 * \file cannon/utils/argparser.hpp
 * \brief File containing ArgParser class definition.
 */

#include <vector>
#include <string>

namespace cannon {
  namespace utils {

    /*!
     * \brief Class which is responsible for parsing command line arguments
     * into a set of inputs and options.
     */
    class ArgParser {
      public:

        /*!
         * \brief Parse the input command line arguments using the options
         * which have been configured on this class.
         *
         * \param argc Number of command line arguments
         * \param argv Array of command line argument strings
         */
        void parse(int argc, char** argv);

        /*!
         * \brief Get all "inputs" parsed by this class, i.e. arguments not
         * interpreted as options.
         *
         * \returns Vector of inputs
         */
        std::vector<std::string> get_inputs() const;

      private:
        std::vector<std::string> inputs_; //!< Arguments not interpreted as

    };

  }
}

#endif /* ifndef CANNON_UTILS_ARGPARSER_H */
