#include <thread>
#include <chrono>

#include "thread_flow.hpp"

namespace sat {

inline ThreadFlow::ThreadFlow()
: m_isRunning(true)
, m_mutex()
, m_runCondition()
{}


inline void ThreadFlow::Start() noexcept
{
    m_isRunning = true;
    m_runCondition.notify_one();
}


inline void ThreadFlow::Stop() noexcept
{
    m_isRunning = false;
}


inline bool ThreadFlow::CheckFlow() noexcept
{
    std::unique_lock<std::mutex> ul(m_mutex);
    while (!m_isRunning) {
        m_runCondition.wait(ul);
    }

    return m_isRunning;
}


inline void ThreadFlow::Nap(size_t a_seconds) const
{
    std::this_thread::sleep_for(std::chrono::seconds(a_seconds));
}


} //sat