#include "garbage_collector.hpp"
#include "muhsat_protocol.hpp"

namespace sat {

GarbageCollector::GarbageCollector(std::shared_ptr<SqliteDatabase> a_hkDatabase)
: m_hkDatabase(a_hkDatabase)
{}


void GarbageCollector::Clean(std::unique_ptr<csp::Packet> a_packet)
{
    std::pair<std::string, uint64_t> info = ExtractInfo(std::move(a_packet));
    if (!info.first.empty()) {
        m_hkDatabase->Delete(info.first, info.second);
    }
}


namespace details {

std::string GetTableName(subsys::SubsystemsT a_subSystem)
{
        switch (a_subSystem) {
        case subsys::COM:
            return comm_table;
        
        case subsys::ADCS:
            return adcs_table;

        case subsys::EPS:
            return eps_table;

        case subsys::TEMP_SENSORS:
            return temp_table;

        case subsys::SUN_SENSORS:
            return sun_table;
        
        default:
            return std::string();
    }
}

} //details


std::pair<std::string, uint64_t> GarbageCollector::ExtractInfo(std::unique_ptr<csp::Packet> a_packet) const noexcept
{
    std::optional<uint8_t> subSystem = Protocol::SubProtocol(*a_packet);
    if (!subSystem.has_value()) {
        return {};
    }

    uint64_t limit = Protocol::UnpackHKDReceivedTimeStamp(*a_packet);
    std::string tableName = details::GetTableName(static_cast<subsys::SubsystemsT>(subSystem.value()));

    return std::make_pair(tableName, limit);
}


} //sat