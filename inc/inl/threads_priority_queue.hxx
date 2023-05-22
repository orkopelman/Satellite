#ifndef THREADS_PRIORITY_QUEUE_HXX
#define THREADS_PRIORITY_QUEUE_HXX

#include <queue>
#include <mutex>
#include <memory>
#include <condition_variable>
#include <vector>

#include "fsl_exception.hpp"

namespace threads {

namespace detail {

static size_t CheckNoZero(size_t a_num) {
    if (!a_num) {
        throw priority_queue_failure("The Capacity can't be set to 0.");
    }
    return a_num;
}

} // detail

template<typename T, class COMPERATOR>
PriorityQueue<T, COMPERATOR>::PriorityQueue(size_t a_capacity, COMPERATOR&& a_funcObj)
: m_priorityQueue(a_funcObj)
, m_mutex()
, m_notEmpty()
, m_notFull()
, m_exited()
, m_terminate(false)
, m_threadsNum(0)
, m_capacity(detail::CheckNoZero(a_capacity))
{}

template<typename T, class COMPERATOR>
PriorityQueue<T, COMPERATOR>::~PriorityQueue()
{
    if (!m_terminate) {
        Shutdown();
    }
    {
        std::unique_lock lock{m_mutex};
    }
}

template<typename T, class COMPERATOR>
void PriorityQueue<T, COMPERATOR>::Enqueue(const T& a_item)
{
    if (m_terminate) {
        throw priority_queue_shutdown("Can't add to priority queue after shutdown.");
    }

    threadEntered();
    std::unique_lock lock{m_mutex};
    while (unsafeIsFull()) {
        if (m_terminate) {
            threadLeft();
            throw priority_queue_shutdown("Shutdown!");
        }
        m_notFull.wait(lock);
        if (m_terminate) {
            threadLeft();
            throw priority_queue_shutdown("Shutdown!");
        }
    }
    try {
        m_priorityQueue.push(a_item);
    } catch (...) {
        threadLeft();
        throw priority_queue_failure("Failed to push item to priority queue.");
    }
    m_notEmpty.notify_all();
    threadLeft();
}

template<typename T, class COMPERATOR>
void PriorityQueue<T, COMPERATOR>::Enqueue(T&& a_item)
{
    if (m_terminate) {
        throw priority_queue_shutdown("Can't add to priority queue after shutdown.");
    }

    threadEntered();
    std::unique_lock lock{m_mutex};
    while (unsafeIsFull()) {
        if (m_terminate) {
            threadLeft();
            throw priority_queue_shutdown("Shutdown!");
        }
        m_notFull.wait(lock);
        if (m_terminate) {
            threadLeft();
            throw priority_queue_shutdown("Shutdown!");
        }
    }
    try {
        m_priorityQueue.push(std::move(a_item));
    } catch (...) {
        threadLeft();
        throw priority_queue_failure("Failed to push item to priority queue.");
    }
    m_notEmpty.notify_all();
    threadLeft();
}

template<typename T, class COMPERATOR>
void PriorityQueue<T, COMPERATOR>::Dequeue(T& a_buffer)
{
    if (m_terminate) {
        throw priority_queue_shutdown("Can't pop from priority queue after shutdown.");
    }

    threadEntered();
    std::unique_lock lock{m_mutex};
    while (unsafeIsEmpty()) {
        if (m_terminate) {
            threadLeft();
            throw priority_queue_shutdown("Shutdown!");
        }
        m_notEmpty.wait(lock);
        if (m_terminate) {
            threadLeft();
            throw priority_queue_shutdown("Shutdown!");
        }
    }
    try {
        a_buffer = m_priorityQueue.top();
        m_priorityQueue.pop();
    } catch (...) {
        threadLeft();
        throw priority_queue_failure("Failed to pop item from priority queue.");
    }
    m_notFull.notify_all();
    threadLeft();
}

template<typename T, class COMPERATOR>
void PriorityQueue<T, COMPERATOR>::Shutdown()
{
    std::unique_lock lock{m_mutex};
    m_terminate = true;
    m_notEmpty.notify_all();
    m_notFull.notify_all();

    while (m_threadsNum > 0) {
        m_exited.wait(lock);
    }
}

template<typename T, class COMPERATOR>
bool PriorityQueue<T, COMPERATOR>::IsEmpty() const noexcept
{
    std::lock_guard lock{m_mutex};
    return unsafeIsEmpty();
}

template<typename T, class COMPERATOR>
bool PriorityQueue<T, COMPERATOR>::IsFull() const noexcept
{
    std::lock_guard lock{m_mutex};
    return unsafeIsFull();
}

template<typename T, class COMPERATOR>
size_t PriorityQueue<T, COMPERATOR>::Size() const noexcept
{
    std::lock_guard lock{m_mutex};
    return m_priorityQueue.size();
}

template<typename T, class COMPERATOR>
bool PriorityQueue<T, COMPERATOR>::unsafeIsEmpty() const noexcept
{
    return m_priorityQueue.size() == 0;
}

template<typename T, class COMPERATOR>
bool PriorityQueue<T, COMPERATOR>::unsafeIsFull() const noexcept
{
    return m_priorityQueue.size() == m_capacity;
}

template<typename T, class COMPERATOR>
void PriorityQueue<T, COMPERATOR>::threadEntered()
{
    ++m_threadsNum;
}

template<typename T, class COMPERATOR>
void PriorityQueue<T, COMPERATOR>::threadLeft()
{
    --m_threadsNum;
    m_exited.notify_one();
}

} // threads

#endif /* THREADS_PRIORITY_QUEUE_HXX */