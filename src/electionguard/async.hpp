#ifndef __ELECTIONGUARD_CPP_ASYNC_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_ASYNC_HPP_INCLUDED__

#include "electionguard/export.h"
#include "log.hpp"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>

using std::atomic;
using std::condition_variable;
using std::future;
using std::future_status;
using std::mutex;
using std::unique_lock;
using std::unique_ptr;
using std::vector;
using std::chrono::seconds;

namespace electionguard
{
    /// <summary>
    /// A simple locking mechanism that allows the compiler to choose
    /// how to implement the deferral.
    ///
    /// use like this:
    /// AsyncSemaphore task_lock(4); //<-- count of threads
    /// auto task = std::async(std::launch::async,
    ///                        [](AsyncSemaphore &mutex) {
    ///                            mutex.wait();
    ///                            return doStuff();
    ///                        },
    ///                        std::ref(task_lock)));
    /// </summary>
    class EG_INTERNAL_API AsyncSemaphore
    {
      public:
        explicit AsyncSemaphore(size_t count) : _count(count) {}
        AsyncSemaphore(const AsyncSemaphore &other) = delete;
        AsyncSemaphore(const AsyncSemaphore &&other) = delete;

        AsyncSemaphore &operator=(AsyncSemaphore other) = delete;
        AsyncSemaphore &operator=(AsyncSemaphore &&other) = delete;

        size_t getCount() const { return _count; };
        void lock()
        {
            unique_lock<mutex> lock(_mutex);
            _condition.wait(lock, [this] { return _count != 0; });
            --_count;
        }
        void unlock()
        {
            unique_lock<mutex> lock(_mutex);
            ++_count;
            _condition.notify_one();
        }

        void wait() { std::scoped_lock _lock(*this); }

      private:
        mutex _mutex;
        condition_variable _condition;
        atomic<size_t> _count;
    };

    template <typename T> EG_INTERNAL_API bool isReady(const future<T> &f)
    {
        if (f.valid()) {
            return f.wait_for(seconds(0)) == future_status::ready;
        } else {
            return false;
        }
    }

    template <typename T>
    EG_INTERNAL_API vector<unique_ptr<T>> wait_all(vector<future<unique_ptr<T>>> &tasks)
    {
        vector<unique_ptr<T>> results;
        do {
            for (auto &task : tasks) {
                if (isReady(task)) {
                    results.push_back(std::move(task.get()));
                }
            }
        } while (results.size() < tasks.size());
        return results;
    }

    /// <summary>
    /// A simple asynchronous thread safe queue using locks.
    /// </summary>
    template <typename T> class EG_INTERNAL_API AsyncQueue
    {
      public:
        AsyncQueue() : _head(new Node), _tail(_head.get()) {}
        AsyncQueue(const AsyncQueue &other) = delete;
        AsyncQueue(const AsyncQueue &&other) = delete;

        AsyncQueue &operator=(AsyncQueue other) = delete;
        AsyncQueue &operator=(AsyncQueue &&other) = delete;

        bool empty()
        {
            std::lock_guard<std::mutex> lock(_head_mutex);
            return (_head.get() == _getTail());
        }

        std::shared_ptr<T> pop()
        {
            const auto old = _tryPop();
            return old ? old->data : std::shared_ptr<T>();
        }

        bool pop(T &value)
        {
            const auto old = _tryPop(value);
            return old != nullptr;
        }

        void push(T value)
        {
            auto data = std::make_shared<T>(std::move(value));
            auto node = std::unique_ptr<Node>(new Node);
            {
                std::lock_guard<std::mutex> lock(_tail_mutex);
                _tail->data = data;
                Node *const new_tail = node.get();
                _tail->next = std::move(node);
                _tail = new_tail;
            }
            _condition.notify_one();
        }

      private:
        struct Node {
            std::shared_ptr<T> data;
            std::unique_ptr<Node> next;
        };

        std::unique_ptr<Node> _head;
        Node *_tail;

        std::mutex _head_mutex;
        std::mutex _tail_mutex;

        std::condition_variable _condition;

        Node *_getTail()
        {
            std::lock_guard<std::mutex> lock(_tail_mutex);
            return _tail;
        }

        std::unique_ptr<Node> _pop()
        {
            if (_head.get() == _getTail()) {
                return nullptr;
            }
            auto old = std::move(_head);
            _head = std::move(old->next);
            return old;
        }

        std::unique_ptr<Node> _tryPop()
        {
            std::lock_guard<std::mutex> lock(_head_mutex);
            if (_head.get() == _getTail()) {
                return std::unique_ptr<Node>();
            }
            return _pop();
        }

        std::unique_ptr<Node> _tryPop(T &value)
        {
            std::lock_guard<std::mutex> lock(_head_mutex);
            if (_head.get() == _getTail()) {
                return std::unique_ptr<Node>();
            }
            value = std::move(*_head->data);
            return _pop();
        }
    };

    /// <summary>
    /// A generic wrapper around a callable type such as a function.
    /// </summary>
    class EG_INTERNAL_API CallableWrapper
    {
      public:
        CallableWrapper() {}
        template <typename F>
        CallableWrapper(F &&callable) : _pimpl(new TypedImpl<F>(std::move(callable)))
        {
        }

        CallableWrapper(CallableWrapper &&other) : _pimpl(std::move(other._pimpl)) {}

        CallableWrapper(const CallableWrapper &other) = delete;

        CallableWrapper &operator=(const CallableWrapper &) = delete;

        CallableWrapper &operator=(CallableWrapper &&other)
        {
            _pimpl = std::move(other._pimpl);
            return *this;
        }

        void operator()() { _pimpl->call(); }

        void call() { _pimpl->call(); }

      private:
        struct Impl {
            virtual void call() = 0;
            virtual ~Impl() {}
        };
        std::unique_ptr<Impl> _pimpl;

        template <typename F> struct TypedImpl : Impl {
            F callable;
            TypedImpl(F &&callable) : callable(std::move(callable)) {}
            void call() { callable(); }
        };
    };

    /// <summary>
    /// A simple thread pool that uses a common work queue for all threads
    /// </summary>
    class ThreadPool
    {
      public:
        ThreadPool(const std::uint_fast32_t &thread_count = std::thread::hardware_concurrency())
            : _thread_count(thread_count)
        {
            try {
                for (unsigned i = 0; i < _thread_count; ++i) {
                    _threads.push_back(std::thread(&ThreadPool::worker, this));
                }
            } catch (...) {
                _running = false;
                throw;
            }
        }

        ThreadPool(const ThreadPool &other) = delete;
        ThreadPool(const ThreadPool &&other) = delete;

        ~ThreadPool()
        {
            wait();
            _running = false;
            for (auto &thread : _threads) {
                thread.join();
            }
        }

        ThreadPool &operator=(ThreadPool other) = delete;
        ThreadPool &operator=(ThreadPool &&other) = delete;

        template <typename F> std::future<typename std::result_of<F()>::type> submit(F callable)
        {
            typedef typename std::result_of<F()>::type result_type;

            std::packaged_task<result_type()> task(std::move(callable));
            std::future<result_type> result(task.get_future());
            _taskQueue.push(std::move(task));
            return result;
        }

      private:
        std::uint_fast32_t _thread_count;
        std::atomic<bool> _running = true;
        AsyncQueue<CallableWrapper> _taskQueue;
        std::mutex queue_mutex = {};
        std::vector<std::thread> _threads;

        void wait()
        {
            while (_running) {
                if (_taskQueue.empty() == true) {
                    break;
                }
                std::this_thread::yield();
            }
        }

        void worker()
        {
            while (_running) {
                CallableWrapper task;
                if (_taskQueue.pop(task)) {
                    task();
                } else {
                    std::this_thread::yield();
                }
            }
        }
    };

    /// <summary>
    /// A singleton instance of a thread pool scheduler
    /// </summary>
    class EG_INTERNAL_API Scheduler
    {
      public:
        Scheduler(Scheduler const &) = delete;
        void operator=(Scheduler const &) = delete;

        static Scheduler &getInstance()
        {
            static Scheduler instance;
            return instance;
        }

        template <typename F> static std::future<typename std::result_of<F()>::type> submit(F task)
        {
            return getInstance()._pool.submit(task);
        }

      private:
        Scheduler() {}
        ThreadPool _pool;
    };

} // namespace electionguard

#endif // __ELECTIONGUARD_CPP_ASYNC_HPP_INCLUDED__
