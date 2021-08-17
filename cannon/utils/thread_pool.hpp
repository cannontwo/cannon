#ifndef CANNON_UTILS_THREAD_POOL_H
#define CANNON_UTILS_THREAD_POOL_H 

/*!
 * \file cannon/utils/thread_pool.hpp
 * \brief File containing ThreadPool class definition.
 */

#include <thread>
#include <vector>
#include <stdexcept>
#include <functional>

#include <cannon/utils/blocking_queue.hpp>

namespace cannon {
  namespace utils {

    /*!
     * \brief Class representing a templated thread pool.
     * Adapted from https://vorbrodt.blog/2019/02/12/simple-thread-pool/
     */
    template <typename T>
    class ThreadPool {
      public:
        ThreadPool() = delete;

        /*!
         * \brief Constructor taking a function for each thread to run on each
         * work item and a number of threads to spawn.
         */
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

        /*!
         * \brief Destructor.
         */
        ~ThreadPool() {
          if (!joined_) {
            queue_.push(nullptr);
            for (auto& thread : threads_)
              thread.join();
          }
        }

        /*!
         * \brief Join all threads in this pool, blocking until they return.
         */
        void join() {
          if (!joined_) {
            queue_.push(nullptr);
            for (auto& thread : threads_)
              thread.join();

            joined_ = true;
          }
        }

        /*!
         * \brief Enqueue work in this pool.
         *
         * \param t The task to enqueue.
         */
        void enqueue(std::shared_ptr<T> t) {
          queue_.push(t);
        }

        /*!
         * \brief Enqueue work in this pool.
         *
         * \param t The task to enqueue.
         */
        void enqueue(T t) {
          queue_.push(std::make_shared<T>(t));
        }

      private:
        std::vector<std::thread> threads_; //!< Work threads
        BlockingQueue<std::shared_ptr<T>> queue_; //!< Task queue
        bool joined_ = false; //!< Whether threads have been joined

    };

  } // namespace utils
} // namespace cannon

#endif /* ifndef CANNON_UTILS_THREAD_POOL_H */
