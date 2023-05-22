#include "collector_intermediate.hpp"
#include "csp_packet.hpp"
#include "muhsat_protocol.hpp"


namespace sat {

CollectorIntermediate::CollectorIntermediate(std::shared_ptr<SqliteDatabase> a_hkDatabase)
: m_hkDatabase(a_hkDatabase)
, m_flowController()
{}


void CollectorIntermediate::Operate() noexcept
{
    while(m_flowController.CheckFlow()) {
        std::unique_ptr<HouseKeepingData> data;
        try {
            std::unique_ptr<HouseKeepingData> temp = Collect();
            data.reset(temp.release());
        } catch(const EmergencyException& e) {
            csp::Packet anomality;
            Protocol::Anomaly(anomality, e.what());
            // send packet to router
        }
        
        WriteToTable(std::move(data));

        size_t napPeriod = waitFor();
        m_flowController.Nap(napPeriod);
    }
}


void CollectorIntermediate::Start() noexcept
{
    m_flowController.Start();
}


void CollectorIntermediate::Stop() noexcept
{
    m_flowController.Stop();
}


} //sat