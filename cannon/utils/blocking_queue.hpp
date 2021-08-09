#ifndef CANNON_UTILS_BLOCKING_QUEUE
#define CANNON_UTILS_BLOCKING_QUEUE 

/*!
 * \namespace cannon::utils
 * \brief Namespace containing various utilities.
 */

/*!
 * \file cannon/utils/blocking_queue.hpp
 * \brief File containing BlockingQueue class definition.
 */

#include <queue>
#include <mutex>
#include <condition_variable>

namespace cannon {
  namespace utils {

    /*!
     * \brief Class representing a blocking thread-safe queue.
     *
     * Adapted from https://gist.github.com/thelinked/6997598
     */
    template <typename T>
    class BlockingQueue {
      public:
        
        /*!
         * \brief Push an item onto the queue, blocking until the queue is
         * available.
         *
         * \param data The data to push
         */
        void push(const T& data) {
          {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(data);

          }

          cond_.notify_one();
        }

        /*!
         * \brief Try popping an item from the queue, returning whether the pop
         * was successful.
         *
         * \param value Reference to place popped value in.
         *
         * \returns Whether a value could be popped.
         */
        bool try_pop(T& value) {
          std::lock_guard<std::mutex> lock(mutex_);
          if (queue_.empty()) {
            return false;
          }

          value = queue_.front();
          queue_.pop();
          return true;
        }

        /*!
         * \brief Attempt to pop a value from the queue, waiting until one is
         * available if the queue is empty.
         *
         * \param value Reference to place popped value in.
         */
        void wait_pop(T& value) {
          std::unique_lock<std::mutex> lock(mutex_);
          while (queue_.empty()) {
            cond_.wait(lock);
          }

          value = queue_.front();
          queue_.pop();
        }

        /*!
         * \brief Get whether the queue is empty.
         *
         * \returns Whether the queue is empty.
         */
        bool empty() const {
          std::lock_guard<std::mutex> lock(mutex_);
          return queue_.empty();
        }

      private:
        std::queue<T> queue_; //!< Internal queue of items
        mutable std::mutex mutex_; //!< Mutex controlling queue access
        std::condition_variable cond_; //!< Condition variable for more efficient access

    };

  } // namespace utils
} // namespace cannon

#endif /* ifndef CANNON_UTILS_BLOCKING_QUEUE */
