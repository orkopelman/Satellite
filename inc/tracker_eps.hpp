#ifndef TRACKER_EPS_HPP
#define TRACKER_EPS_HPP

#include <memory>

#include "interface_trackable_event.hpp"
#include "intermediate_tracker.hpp"

namespace sat {

class EPSTracker : public IntermediateTracker {
public:
    using IntermediateTracker::IntermediateTracker;

    size_t IntermissionLength() const noexcept override;
    std::string TableName() const noexcept override;
    std::string CollectData() const noexcept override;
    std::shared_ptr<csp::Packet> Pack(const std::string& a_rawData) const noexcept; 
};



} // sat

#endif //TRACKER_EPS_HPP

