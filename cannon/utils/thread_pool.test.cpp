#include <thread>
#include <chrono>
#include <cassert>

#include <cannon/utils/thread_pool.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::utils;
using namespace cannon::log;

int main() {
  ThreadPool<int> tp([&](std::shared_ptr<int> x){
        assert(x != nullptr);
        assert(x >= 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(*x));
        log_info("Thread slept for", *x, "milliseconds");
        tp.enqueue(x);
      });

  tp.enqueue(10);
  tp.enqueue(17);
  tp.enqueue(100);

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
