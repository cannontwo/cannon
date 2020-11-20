#include <cassert>
#include <thread>

#include <cannon/utils/blocking_queue.hpp>

using namespace cannon::utils;

int main() {
  BlockingQueue<int> q;

  int value;

  assert(!q.try_pop(value));
  q.push(1);
  assert(q.try_pop(value));
  assert(value == 1);

  auto t1 = std::thread([&](){
              q.push(2);
            });

  auto t2 = std::thread([&]() {
        int value_2;
        q.wait_pop(value_2);
        assert(value_2 == 2);
      });
  
  t1.join();
  t2.join();
}
