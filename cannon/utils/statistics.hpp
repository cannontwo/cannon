#pragma once
#ifndef CANNON_UTILS_STATISTICS_H
#define CANNON_UTILS_STATISTICS_H 

/*!
 * \file cannon/utils/statistics.hpp
 * File containing statistics collecting utilities. Adapted from PBRT Ch. A.7.
 */

// Variable titles should be of the format "category/name"

#define STAT_COUNTER(title, var)                        \
static thread_local int64_t var;                        \
static void STATS_FUNC##var(StatsAccumulator &accum) {  \
  accum.ReportCounter(title, var);                      \
  var = 0;                                              \
}                                                       \
static StatRegisterer STATS_REG##var(STATS_FUNC##var)   

#include <cstring>
#include <string>
#include <map>
#include <functional>
#include <mutex>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>

namespace cannon {
  namespace utils {

    /*!
     * Function to parse variable name into separate category and variable title.
     *
     * \param str Combined variable name to parse.
     * \param category Pointer in which to store category portion of name.
     * \param title Pointer in which to store title portion of name.
     */
    void get_category_and_title(const std::string& str, std::string *category, std::string *title);

    /*!
     * \brief Class that accumulates different statistic counters.
     */
    class StatsAccumulator {
      public:

        /*!
         * Report the results of a counter variable with the given value.
         *
         * \param name The name of the counter
         * \param val The counter value
         */
        void ReportCounter(const std::string& name, int64_t val) {
          counters_[name] += val; 
        }

        /*!
         * Print all accumulated statistics to the input filename.
         *
         * \param filename The file to write statistics to.
         */
        void print(const std::string& filename) {
          std::ofstream out_file(filename);
          std::map<std::string, std::vector<std::string>> to_print;

          out_file << "Statistics:" << std::endl;

          for (auto& counter : counters_) {
            if (counter.second == 0) continue;

            std::string category, title;
            get_category_and_title(counter.first, &category, &title);

            std::stringstream ss;
            ss << std::setw(42) << std::left << title;
            ss << std::setw(12) << counter.second;

            to_print[category].push_back(ss.str());
          }

          // TODO More kinds of counters
          
          for (auto& categories : to_print) {
            out_file << "  " << categories.first << std::endl;
            for (auto& item : categories.second) {
              out_file << "  \t" << item << std::endl;
            }
          }
        }

      public:
        std::map<std::string, int64_t> counters_; //!< Counters for registered statistics

    };

    /*!
     * Class that collects all of the various thread-local statistic reporters for a single variable.
     */
    class StatRegisterer {
      public:

        /*!
         * Constructor taking a statistic accumulating function.
         */
        StatRegisterer(std::function<void(StatsAccumulator &)> func);

        /*!
         * Call all registered callbacks to add to the input accumulator.
         */
        static void CallCallbacks(StatsAccumulator &accum);

      private: 
        static std::vector<std::function<void(StatsAccumulator &)>> *funcs_; //!< Statistic accumulating functions

    };

    // Public Functions
    
    static StatsAccumulator stats_accumulator; //!< Overall stats accumulator for all threads
    
    /*!
     * Function to report stats for this thread.
     */
    void report_thread_stats(); 

    /*!
     * Function to print stats to input filename.
     */
    void print_stats(const std::string& filename);

  } // namespace utils
} // namespace cannon

#endif /* ifndef CANNON_UTILS_STATISTICS_H */
