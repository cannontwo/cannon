#include <catch2/catch.hpp>

#include <cannon/utils/experiment_writer.hpp>

using namespace cannon::utils;

TEST_CASE("ExperimentWriter", "[utils]") {
  std::filesystem::remove_all("testing/logs");

  ExperimentWriter writer("testing/logs/");

  REQUIRE(std::filesystem::exists("testing/logs"));
  REQUIRE(std::filesystem::status("testing/logs").type() == std::filesystem::file_type::directory);

  writer.start_log("foo");
  
  REQUIRE(std::filesystem::exists("testing/logs/foo_0.csv"));
  REQUIRE(std::filesystem::status("testing/logs/foo_0.csv").type() == std::filesystem::file_type::regular);

  writer.write_line("foo", "Hello world");
  std::ifstream ifs("testing/logs/foo_0.csv");
  std::string foo_content((std::istreambuf_iterator<char>(ifs)),
                          (std::istreambuf_iterator<char>()));

  REQUIRE(foo_content.compare("Hello world\n") == 0);

  writer.start_log("bar", ".txt");
  writer.start_log("foo");

  REQUIRE(std::filesystem::exists("testing/logs/foo_1.csv"));
  REQUIRE(std::filesystem::exists("testing/logs/bar_0.txt"));

  writer.write_line("foo", "Goodbye world");

  std::ifstream ifs0("testing/logs/foo_0.csv");
  std::string foo0_content((std::istreambuf_iterator<char>(ifs0)),
                          (std::istreambuf_iterator<char>()));

  REQUIRE(foo0_content.compare("Hello world\n") == 0);

  std::ifstream ifs1("testing/logs/foo_1.csv");
  std::string foo1_content((std::istreambuf_iterator<char>(ifs1)),
                          (std::istreambuf_iterator<char>()));

  REQUIRE(foo1_content.compare("Goodbye world\n") == 0);
}
