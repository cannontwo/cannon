#pragma once
#ifndef CANNON_UTILS_EXPERIMENT_RUNNER
#define CANNON_UTILS_EXPERIMENT_RUNNER

/*!
 * \file cannon/utils/experiment_runner.hpp
 * File containing ExperimentRunner class definition.
 */

#include <functional>
#include <random>
#include <vector>

#include <cannon/utils/experiment_writer.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::log;

namespace cannon {
  namespace utils {

    /*!
     * \brief Class that allows for the easy specification and recording of
     * multiple runs of an experiment, which is assumed to have a pseudo-random
     * component which can be controlled by a seed.
     */
    class ExperimentRunner {
      public:
        ExperimentRunner() = delete;

        /*!
         * Constructor taking a log directory prefix, a number of experiments
         * to run, and an experiment function which accepts an experiment
         * writer and a random seed to use.
         */
        ExperimentRunner(const std::string& log_prefix, unsigned int num_exps,
            std::function<void(ExperimentWriter&, int)> exp_func) :
          exp_func_(exp_func), num_exps_(num_exps), log_prefix_(log_prefix),
          start_time_string_(std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()))
      {}

        /*!
         * Constructor taking a log directory prefix, a number of experiments
         * to run, a vector of seeds to use for the experiments, and an
         * experiment function.
         */
        ExperimentRunner(const std::string& log_prefix, unsigned int num_exps,
            std::vector<int> seeds, std::function<void(ExperimentWriter&, int)>
            exp_func) : exp_func_(exp_func), num_exps_(num_exps),
        log_prefix_(log_prefix), seeds_(seeds),
        start_time_string_(std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()))
        {

          if (seeds.size() != num_exps)
            throw std::runtime_error("Constructor passed incorrect number of random seeds.");

        }

        /*!
         * Run all num_exps_ experiments specified by this object.
         */
        void run() {
          static std::random_device rd;

          for (unsigned int i = 0; i < num_exps_; i++) {
            ExperimentWriter writer(log_prefix_ + "/" + start_time_string_ +
                "/run_" + std::to_string(i) + "/");

            if (seeds_.empty()) {
              int seed = rd();
              log_info("Running experiment", i, "with seed", seed);
              exp_func_(writer, seed);
            } else {
              log_info("Running experiment", i, "with seed", seeds_[i]);
              exp_func_(writer, seeds_[i]);
            }
          }

        }

      public: 
        std::function<void(ExperimentWriter&, int)> exp_func_;
        unsigned int num_exps_;
        std::string log_prefix_;
        std::vector<int> seeds_;
        std::string start_time_string_;

    };

  } // namespace utils
} // namespace cannon

#endif /* ifndef CANNON_UTILS_EXPERIMENT_RUNNER */
