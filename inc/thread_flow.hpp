#ifndef THREAD_FLOW_HPP
#define THREAD_FLOW_HPP

#include <atomic>
#include <mutex>
#include <condition_variable>

namespace sat {

class ThreadFlow {
public:
    ThreadFlow();
    ThreadFlow(const ThreadFlow& a_other) = default;
    ThreadFlow& operator=(const ThreadFlow& a_other) = default;
    ~ThreadFlow() = default;

    void Start() noexcept;
    void Stop() noexcept;
    bool CheckFlow() noexcept;
    void Nap(size_t a_seconds) const;

private:
    std::atomic_bool m_isRunning;
    std::mutex m_mutex;
    std::condition_variable m_runCondition;
};

} //sat

#include "inl/thread_flow.hxx"

#endif