#ifndef TRACKER_TEMPRATURE_HPP
#define TRACKER_TEMPRATURE_HPP

#include <memory>

#include "interface_trackable_event.hpp"
#include "intermediate_tracker.hpp"

///#include "csp communication.hpp"

namespace sat {

class TempratureTracker : public IntermediateTracker {
public:
    using IntermediateTracker::IntermediateTracker;

    size_t IntermissionLength() const noexcept override;
    const char* TableName() const noexcept override;
    std::string CollectData() const noexcept override;
    
};

} // sat

#endif //TRACKER_TEMPRATURE_HPP

