#include <catch2/catch.hpp>
#include <thread>

#include <cannon/utils/blocking_queue.hpp>

using namespace cannon::utils;

TEST_CASE("BlockingQueue", "[utils]") {
  BlockingQueue<int> q;

  int value;

  REQUIRE(!q.try_pop(value));
  q.push(1);
  REQUIRE(q.try_pop(value));
  REQUIRE(value == 1);

  auto t1 = std::thread([&](){
              q.push(2);
            });

  auto t2 = std::thread([&]() {
        int value_2;
        q.wait_pop(value_2);
        REQUIRE(value_2 == 2);
      });
  
  t1.join();
  t2.join();
}
