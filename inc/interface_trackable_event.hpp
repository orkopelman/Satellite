#ifndef INTERFACE_TRACKABLE_EVENT_HPP
#define INTERFACE_TRACKABLE_EVENT_HPP

#include <memory>

#include "csp_packet.hpp"
#include "spam.h"

namespace sat {

namespace event {

#define TRACKABLE_EVENTS(_) \
\
    /* Sun Events */ \
    _(SOLAR_ECLIPSE, 0) \
    _(PARTIAL_ECLIPSE, 1) \
    _(FULL_SUN_EXPOSURE, 2) \
    /* EPS Events */ \
    _(CRITICAL_EPS, 3) \
    _(NORMAL_EPS, 4) \
    _(SAFE_EPS, 5) \
    /* Tempratures Events */ \
    _(NORMAL_TEMP, 6) \
    _(SAFE_LOW_TEMP, 7) \
    _(SAFE_HIGH_TEMP, 8) 

SPAM(TRACKABLE_EVENTS, EventT);

} // event 

class InterfaceTrackableEvent {
public:
    virtual ~InterfaceTrackableEvent() = default;

    virtual std::unique_ptr<csp::Packet> Details() const noexcept = 0;
    virtual bool IsNewEvent(std::shared_ptr<csp::Packet> a_currentReading) noexcept = 0;
    
protected:
    InterfaceTrackableEvent() = default;
    InterfaceTrackableEvent(InterfaceTrackableEvent &a_other) = default;
    InterfaceTrackableEvent& operator=(const InterfaceTrackableEvent &a_other) = default;
};

} // sat

#endif //INTERFACE_TRACKABLE_EVENT_HPP


