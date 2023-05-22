#include "intermediate_trackable_event.hpp"
#include "muhsat_protocol.hpp"

namespace sat {

std::unique_ptr<SubSystemData> IntermediateTrackableEvent::SubData(std::shared_ptr<csp::Packet> a_currentReading)    
{
    std::unique_ptr<Protocol::HouseKeepingDataBlock> hkdv = Protocol::UnpackHouseKeepingData(*a_currentReading);
    std::shared_ptr<HouseKeepingData> ptrHKD = (*hkdv)[0];
    std::unique_ptr<SubSystemData> sub_data (std::move(ptrHKD->sub_data));
    return sub_data; 
}

IntermediateTrackableEvent::IntermediateTrackableEvent()
: m_lastResult(false)
{}

bool IntermediateTrackableEvent::IsNewEvent(std::shared_ptr<csp::Packet> a_currentReading) noexcept
{
    bool lastRes = m_lastResult;
    bool isEventPerCriteria = IsEvent(a_currentReading);
    m_lastResult = isEventPerCriteria;
    return (isEventPerCriteria && !lastRes) ? true : false;
}

std::unique_ptr<csp::Packet> IntermediateTrackableEvent::Details() const noexcept
{
    std::unique_ptr<csp::Packet> packetToSend = std::make_unique<csp::Packet>();
    Protocol::EventOccured(*packetToSend, Track(), Event());
    return std::move(packetToSend);
}


} // sat



