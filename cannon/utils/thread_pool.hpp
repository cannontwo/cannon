#ifndef CANNON_UTILS_THREAD_POOL_H
#define CANNON_UTILS_THREAD_POOL_H 

#include <thread>
#include <vector>
#include <stdexcept>
#include <functional>

#include <cannon/utils/blocking_queue.hpp>

namespace cannon {
  namespace utils {

    // Adapted from https://vorbrodt.blog/2019/02/12/simple-thread-pool/
    template <typename T>
    class ThreadPool {
      public:
        ThreadPool() = delete;

        ThreadPool(std::function<void(std::shared_ptr<T>)> f, int num_threads=4) {
          if (num_threads <= 0)
            throw std::runtime_error("ThreadPool created with improper number of threads");

          for (int i = 0; i < num_threads; i++) {
            threads_.emplace_back([this, f](){
                  while (true) {
                    std::shared_ptr<T> item;
                    queue_.wait_pop(item);

                    if (item == nullptr) {
                      queue_.push(nullptr);
                      break;
                    }

                    f(item);
                  }
                });
          }
        }

        ThreadPool(ThreadPool& o) = delete;
        ThreadPool(ThreadPool&& o) = delete;

        ~ThreadPool() {
          if (!joined_) {
            queue_.push(nullptr);
            for (auto& thread : threads_)
              thread.join();
          }
        }

        void join() {
          queue_.push(nullptr);
          for (auto& thread : threads_)
            thread.join();

          joined_ = true;
        }

        void enqueue(std::shared_ptr<T> t) {
          queue_.push(t);
        }

        void enqueue(T t) {
          queue_.push(std::make_shared<T>(t));
        }

      private:
        std::vector<std::thread> threads_;
        BlockingQueue<std::shared_ptr<T>> queue_;
        bool joined_ = false;

    };

  } // namespace utils
} // namespace cannon

#endif /* ifndef CANNON_UTILS_THREAD_POOL_H */
