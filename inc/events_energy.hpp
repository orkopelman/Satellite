#ifndef EVENTS_ENERGY_HPP
#define EVENTS_ENERGY_HPP

#include "interface_trackable_event.hpp"

namespace sat {

class CriticalEPS : public InterfaceTrackableEvent{
public:
    using InterfaceTrackableEvent::InterfaceTrackableEvent;
    
    virtual bool IsEvent(std::shared_ptr<CSPpacket> a_criteria) const noexcept override;
};

class SafeEPS : public InterfaceTrackableEvent{
public:
    using InterfaceTrackableEvent::InterfaceTrackableEvent;
    
    virtual bool IsEvent(std::shared_ptr<CSPpacket> a_criteria) const noexcept override;
};

class NormalEPS : public InterfaceTrackableEvent{
public:
    using InterfaceTrackableEvent::InterfaceTrackableEvent;
    
    virtual bool IsEvent(std::shared_ptr<CSPpacket> a_criteria) const noexcept override;
};

} // sat

#endif //EVENTS_ENERGY_HPP

