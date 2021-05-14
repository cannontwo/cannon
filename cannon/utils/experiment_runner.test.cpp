#include <catch2/catch.hpp>

#include <cannon/utils/experiment_runner.hpp>

using namespace cannon::utils;

TEST_CASE("ExperimentRunner", "[utils]") {
  std::filesystem::remove_all("testing/logs");

  ExperimentRunner runner("testing/logs/", 10, [](ExperimentWriter& w, int s) {
      auto f = w.get_file("seed.txt");
      f << s;
      f.close();
      });

  runner.run();

  for (int i = 0; i < 10; i++) {
    REQUIRE(std::filesystem::exists("testing/logs/" + runner.start_time_string_
          + "/run_" + std::to_string(i) +  "/")); 

    REQUIRE(std::filesystem::status("testing/logs/" + runner.start_time_string_
          + "/run_" + std::to_string(i) +  "/").type() ==
        std::filesystem::file_type::directory);

    REQUIRE(std::filesystem::exists("testing/logs/" + runner.start_time_string_
          + "/run_" + std::to_string(i) +  "/seed.txt"));
  }
}
