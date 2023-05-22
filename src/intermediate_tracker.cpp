#include <algorithm> //for_each

#include "intermediate_tracker.hpp"

namespace sat{

IntermediateTracker::IntermediateTracker(uint16_t a_destAddress, uint8_t a_destPort, const SqliteDatabase& a_db)
: m_db(a_db)
, m_socket(a_destAddress, a_destPort)
{}

void IntermediateTracker::Track(std::shared_ptr<InterfaceTrackableEvent> a_trackableEvent)
{
    std::unique_lock lock(m_mutex);
    m_events.push_back(a_trackableEvent);
}

void IntermediateTracker::Operate() noexcept 
{
    while (m_threadFlow.CheckFlow()) {
        m_threadFlow.Nap(IntermissionLength());
        std::string rawData = CollectData();
        std::shared_ptr<csp::Packet> currentData;
        std::shared_ptr<HouseKeepingData> HKD = std::make_shared<HouseKeepingData>(SubSys(), rawData);
        Protocol::HouseKeepingDataBlock hkdb = {HKD};
        Protocol::PackHouseKeepingData(*currentData, SubSys(), hkdb);
        std::shared_lock lock(m_mutex);
        std::for_each(m_events.begin(), m_events.end(), [&socket = m_socket, &currentData]

                                                        (std::shared_ptr<InterfaceTrackableEvent> a_event){
            if (a_event->IsNewEvent(currentData)) {
                socket.Send((a_event->Details()));
            }
        });
    }
}

void IntermediateTracker::Start() noexcept
{
    m_threadFlow.Start();
}

void IntermediateTracker::Stop() noexcept
{
    m_threadFlow.Stop();
}

} // sat