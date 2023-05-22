#ifndef INTERMEDIATE_TRACKEABLE_EVENT_HPP
#define INTERMEDIATE_TRACKEABLE_EVENT_HPP

#include <atomic>
#include <shared_mutex>

#include "interface_trackable_event.hpp"
#include "housekeeping_data.hpp"

namespace sat {

class IntermediateTrackableEvent : public InterfaceTrackableEvent {
public:
    IntermediateTrackableEvent();
    ~IntermediateTrackableEvent() = default;

    virtual bool IsNewEvent(std::shared_ptr<csp::Packet> a_currentReading) noexcept override;
    virtual std::unique_ptr<csp::Packet> Details() const noexcept override; // pack
    static std::unique_ptr<SubSystemData> SubData(std::shared_ptr<csp::Packet> a_currentReading);  

protected:
    IntermediateTrackableEvent(IntermediateTrackableEvent& const a_ite) = default;
    IntermediateTrackableEvent& operator=(const IntermediateTrackableEvent& a_other) = default;

    virtual bool IsEvent(std::shared_ptr<csp::Packet> a_currentReading) noexcept = 0;

    virtual track::TrackerT Track() const noexcept  = 0;
    virtual event::EventT Event() const noexcept  = 0;

private:
    std::atomic_bool m_lastResult;
};

} // sat

#endif //INTERMEDIATE_TRACKEABLE_EVENT_HPP

