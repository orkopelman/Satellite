#ifndef EVENTS_TEMPRATURE_HPP
#define EVENTS_TEMPRATURE_HPP

#include "interface_trackable_event.hpp"

namespace sat {

class SafeHighTemp : public InterfaceTrackableEvent{
public:
    using InterfaceTrackableEvent::InterfaceTrackableEvent;
    
    virtual CSPpacket Detalis() const noexcept override;
    virtual bool IsEvent(std::shared_ptr<CSPpacket> a_criteria) const noexcept override;
};

class SafeLowTemp : public InterfaceTrackableEvent{
public:
    using InterfaceTrackableEvent::InterfaceTrackableEvent;
    
    virtual CSPpacket Detalis() const noexcept override;
    virtual bool IsEvent(std::shared_ptr<CSPpacket> a_criteria) const noexcept override;
};

class NormalTemp : public InterfaceTrackableEvent{
public:
    using InterfaceTrackableEvent::InterfaceTrackableEvent;
    
    virtual CSPpacket Detalis() const noexcept override;
    virtual bool IsEvent(std::shared_ptr<CSPpacket> a_criteria) const noexcept override;
};

} // sat

#endif //EVENTS_TEMPRATURE_HPP

