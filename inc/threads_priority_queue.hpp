#ifndef THREADS_PRIORITY_QUEUE_HPP
#define THREADS_PRIORITY_QUEUE_HPP

#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <functional>
#include <atomic>

#include "fsl_exception.hpp"

namespace threads {

class priority_queue_failure : public fsl::standard_exception {
public:
    using standard_exception::standard_exception;
};

class priority_queue_shutdown : public fsl::standard_exception {
public:
    using standard_exception::standard_exception;
};

template<typename T, class COMPERATOR = std::less<T> >
class PriorityQueue {
public:
    PriorityQueue(size_t a_capacity, COMPERATOR&& a_comperator = COMPERATOR{});
    PriorityQueue(const PriorityQueue& a_other) = delete;
    PriorityQueue& operator=(const PriorityQueue& a_other) = delete;
    ~PriorityQueue();

    void Enqueue(const T& a_item);
    void Enqueue(T&& a_item);

    void Dequeue(T& a_buffer);

    void Shutdown();

    bool IsEmpty() const noexcept;
    bool IsFull() const noexcept;

    size_t Size() const noexcept;

private:
    bool unsafeIsEmpty() const noexcept;
    bool unsafeIsFull() const noexcept;

    void threadEntered();
    void threadLeft();

private:
    using container = std::priority_queue<T, std::vector<T>, COMPERATOR>;

    container m_priorityQueue;
    mutable std::mutex m_mutex;
    std::condition_variable m_notEmpty;
    std::condition_variable m_notFull;
    std::condition_variable m_exited;
    std::atomic<bool> m_terminate;
    std::atomic<size_t> m_threadsNum;
    size_t m_capacity;
};

} // threads

#include "inl/threads_priority_queue.hxx"

#endif /* THREADS_PRIORITY_QUEUE_HPP */