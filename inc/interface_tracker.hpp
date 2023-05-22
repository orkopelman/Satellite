#ifndef INTEFACE_TRACKER_HPP
#define INTEFACE_TRACKER_HPP

#include <memory>

#include "interface_trackable_event.hpp"
#include "spam.h"

namespace sat {

namespace track {

#define TRACKERS(_) \
    _(SUN_TRACKER, 0) \
    _(ENERGY_TRACKER, 1) \
    _(TEMP_TRACKER, 2)

SPAM(TRACKERS, TrackerT);

} // track

class InterfaceTracker {
public:
    ~InterfaceTracker() = default;

    virtual void Operate() noexcept = 0 ; //run thread
    virtual void Track(std::shared_ptr<InterfaceTrackableEvent> a_trackableEvent) = 0;
    virtual void Start() noexcept = 0;
    virtual void Stop() noexcept = 0;
  
protected:
    InterfaceTracker() = default;
    InterfaceTracker(const InterfaceTracker& bf) = default;
    InterfaceTracker& operator=(const InterfaceTracker& a_other) = default;

};

} // sat

#endif //INTEFACE_TRACKER_HPP


