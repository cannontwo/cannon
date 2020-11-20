#ifndef CANNON_UTILS_BLOCKING_QUEUE
#define CANNON_UTILS_BLOCKING_QUEUE 

#include <queue>
#include <mutex>
#include <condition_variable>

namespace cannon {
  namespace utils {

    // Adapted from https://gist.github.com/thelinked/6997598
    template <typename T>
    class BlockingQueue {
      public:
        void push(const T& data) {
          {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(data);

          }

          cond_.notify_one();
        }

        bool try_pop(T& value) {
          std::lock_guard<std::mutex> lock(mutex_);
          if (queue_.empty()) {
            return false;
          }

          value = queue_.front();
          queue_.pop();
          return true;
        }

        void wait_pop(T& value) {
          std::unique_lock<std::mutex> lock(mutex_);
          while (queue_.empty()) {
            cond_.wait(lock);
          }

          value = queue_.front();
          queue_.pop();
        }

        bool empty() const {
          std::lock_guard<std::mutex> lock(mutex_);
          return queue_.empty();
        }

      private:
        std::queue<T> queue_;
        mutable std::mutex mutex_;
        std::condition_variable cond_;

    };

  } // namespace utils
} // namespace cannon

#endif /* ifndef CANNON_UTILS_BLOCKING_QUEUE */
