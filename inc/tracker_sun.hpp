#ifndef TRACKER_SUN_HPP
#define TRACKER_SUN_HPP

#include <memory>

#include "interface_trackable_event.hpp"
#include "intermediate_tracker.hpp"


namespace sat {

class SunTracker : public IntermediateTracker {
public:
    using IntermediateTracker::IntermediateTracker;

    size_t IntermissionLength() const noexcept override;
    const char* TableName() const noexcept override;
    std::string CollectData() const noexcept override;
    subsys::SubsystemsT SubSys() const noexcept override;
};

} // sat

#include "inl/tracker_sun.hxx"

#endif //TRACKER_SUN_HPP

