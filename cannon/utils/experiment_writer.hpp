#pragma once
#ifndef CANNON_UTILS_EXPERIMENT_WRITER_H
#define CANNON_UTILS_EXPERIMENT_WRITER_H 

/*!
 * \file cannon/utils/experiment_writer.hpp
 * \brief File containing ExperimentWriter class definition.
 */

#include <string>
#include <filesystem>
#include <map>
#include <iostream>
#include <fstream>
#include <stdexcept>

namespace cannon {
  namespace utils {

    /*!
     * \brief Class abstracting writing logs for an experiment that might
     * create several different files with shared prefixes.
     */
    class ExperimentWriter {
      public:
        ExperimentWriter() = delete;

        /*!
         * Constructor taking parent directory for writing logs.
         */
        ExperimentWriter(const std::string& parent_dir) : parent_dir_(parent_dir) {
          std::filesystem::create_directories(parent_dir_);
        }

        /*!
         * Start a log by creating the first corresponding file. Note that log
         * name should not contain file extensions. If the log has already been
         * started, the current log file will be closed and a new one with an
         * incremented number will be opened.
         *
         * \param log_name Name of log to start
         * \param extension File extension for log files
         */
        void start_log(const std::string& log_name, const std::string& extension=".csv") {
          if (logs_.find(log_name) == logs_.end()) {
            // Only add to maps if this log doesn't already exist
            log_nums_[log_name] = 0;

            std::ofstream log_file(parent_dir_ + "/" + log_name + "_" +
                std::to_string(log_nums_[log_name]) + extension);
            logs_[log_name] = std::move(log_file);

            extensions_[log_name] = extension;
          } else {
            // If log already exists, close existing file, increment num, and reopen
            log_nums_[log_name]++;

            std::ofstream log_file(parent_dir_ + "/" + log_name + "_" +
                std::to_string(log_nums_[log_name]) + extensions_[log_name]);
            logs_[log_name].close();
            logs_[log_name] = std::move(log_file);
          }
        }

        /*!
         * Write a line to a log. Throws an exception if the log is not already open.
         *
         * \param log_name The log to write to.
         * \param entry The string to write.
         */
        void write_line(const std::string& log_name, const std::string& entry) {
          if (logs_.find(log_name) == logs_.end())
            throw std::runtime_error("Log has not been previously opened.");

          logs_[log_name] << entry << std::endl;
        }

        /*!
         * Create and return a file stream in the directory managed by this
         * object. This is convenient for storing single-use information about
         * an experiment for which a formal log is not needed.
         *
         * \param filename Name of file in experiment directory.
         *
         * \returns An opened file with the input name.
         */
        std::ofstream get_file(const std::string& filename) {
          std::ofstream file(parent_dir_ + "/" + filename);
          return file;
        }

        /*!
         * Create subdirectory with the given name in experiment log directory
         * managed by this object.
         * 
         * \param subdir_name Name of subdirectory to create.
         */
        void create_subdir(const std::string& subdir_name) {
          std::filesystem::create_directories(parent_dir_ + "/" + subdir_name);
        }

      private:
        std::string parent_dir_; //!< Parent directory under which logs should be placed

        std::map<std::string, std::ofstream> logs_; //!< Open log files
        std::map<std::string, std::string> extensions_; //!< File extensions for opened logs
        std::map<std::string, unsigned int> log_nums_; //!< Number of opened log

    };

  } // namespace utils
} // namespace cannon

#endif /* ifndef CANNON_UTILS_EXPERIMENT_WRITER_H */
