#include "housekeeping_data.hpp"
#include "parser.hpp"
#include "csp_packet.hpp"
#include "muhsat_protocol.hpp"

namespace sat {


namespace details {
    
uint8_t SubSystemMembers(subsys::SubsystemsT a_subsystem)
{
    switch (a_subsystem) {
        case subsys::COM:
            return comm_members;
        
        case subsys::ADCS:
            return adcs_members;

        case subsys::EPS:
            return eps_members;

        case subsys::TEMP_SENSORS:
            return temp_members;

        case subsys::SUN_SENSORS:
            return sun_members;
        
        default:
            return 0;
    }
}


std::unique_ptr<CommData> BuildComData(std::vector<std::string>& a_membersData)
{
    std::unique_ptr<CommData> data = std::make_unique<CommData>();

    data->ipAddress = std::stoi(a_membersData[0].c_str());
    data->port = std::stoi(a_membersData[1].c_str());
    data->packetSize = std::stoll(a_membersData[2].c_str());

    return data;
}


std::unique_ptr<AdcsData> BuildAdcsData(std::vector<std::string>& a_membersData)
{
    std::unique_ptr<AdcsData> data = std::make_unique<AdcsData>();

    data->xrotationAngle = std::stod(a_membersData[0].c_str());
    data->yrotationAngle = std::stod(a_membersData[1].c_str());
    data->zrotationAngle = std::stod(a_membersData[2].c_str());

    return data;
}


std::unique_ptr<EpsData> BuildEpsData(std::vector<std::string>& a_membersData)
{
    std::unique_ptr<EpsData> data = std::make_unique<EpsData>();

    data->batteryPercentage = std::stod(a_membersData[0].c_str());
    data->voltage = std::stod(a_membersData[1].c_str());
    data->current = std::stod(a_membersData[1].c_str());

    return data;
}


std::unique_ptr<TempData> BuildTempData(std::vector<std::string>& a_membersData)
{
    std::unique_ptr<TempData> data = std::make_unique<TempData>();

    data->temperatue = std::stod(a_membersData[0].c_str());

    return data;
}


std::unique_ptr<SunData> BuildSunData(std::vector<std::string>& a_membersData)
{
    std::unique_ptr<SunData> data = std::make_unique<SunData>();

    data->xAxisAngle = std::stod(a_membersData[0].c_str());
    data->yAxisAngle = std::stod(a_membersData[1].c_str());
    data->zAxisAngle = std::stod(a_membersData[2].c_str());
    
    return data;
}


std::unique_ptr<SubSystemData> BuildSubData(subsys::SubsystemsT a_subsystem, std::vector<std::string>& a_membersData)
{
    switch (a_subsystem) {
        case subsys::COM:
            return BuildComData(a_membersData);
        
        case subsys::ADCS:
            return BuildAdcsData(a_membersData);

        case subsys::EPS:
            return BuildEpsData(a_membersData);

        case subsys::TEMP_SENSORS:
            return BuildTempData(a_membersData);

        case subsys::SUN_SENSORS:
            return BuildSunData(a_membersData);
        
        default:
            return nullptr;
    }
}

} //details


HouseKeepingData::HouseKeepingData(std::unique_ptr<SubSystemData> a_subData)
: curTime(time(nullptr))
, sub_data(std::move(a_subData)) 
{}


HouseKeepingData::HouseKeepingData(subsys::SubsystemsT a_subsystem, std::string& a_readData)
: curTime()
, sub_data()
{
    size_t numOfMembers = details::SubSystemMembers(a_subsystem);
    std::vector<std::string> vec = Parser::ParseLine(a_readData.data(), numOfMembers);
    
    curTime = std::stoll(vec[0].c_str());
    vec.erase(vec.begin());
    sub_data.reset((details::BuildSubData(a_subsystem, vec).release()));
}

// packing

void HouseKeepingData::Pack(csp::Packet& a_packet) const
{
    Protocol::pack<uint64_t>(a_packet, curTime);
    sub_data->Pack(a_packet);
}

void CommData::Pack(csp::Packet& a_packet) const
{
    Protocol::pack<uint16_t>(a_packet, ipAddress);
    Protocol::pack<uint8_t>(a_packet, port);
    Protocol::pack<uint64_t>(a_packet, packetSize);
}

void AdcsData::Pack(csp::Packet& a_packet) const
{
    Protocol::pack<double>(a_packet, xrotationAngle);
    Protocol::pack<double>(a_packet, yrotationAngle);
    Protocol::pack<double>(a_packet, zrotationAngle);
}

void EpsData::Pack(csp::Packet& a_packet) const
{
    Protocol::pack<double>(a_packet, batteryPercentage);
    Protocol::pack<double>(a_packet, voltage);
    Protocol::pack<double>(a_packet, current);
}

void TempData::Pack(csp::Packet& a_packet) const
{
    Protocol::pack<double>(a_packet, temperatue);
}

void SunData::Pack(csp::Packet& a_packet) const
{
    Protocol::pack<double>(a_packet, xAxisAngle);
    Protocol::pack<double>(a_packet, yAxisAngle);
    Protocol::pack<double>(a_packet, zAxisAngle);
}

// unpackers

HouseKeepingData::HouseKeepingData(const csp::Packet& a_packet, const uint8_t** a_reader, subsys::SubsystemsT a_HKsubsys)
: curTime(Protocol::unpack<uint64_t>(a_packet, a_reader))
, sub_data(nullptr)
{
    switch (a_HKsubsys) {
        case subsys::COM:
            sub_data = std::make_unique<CommData>(a_packet, a_reader);
            break;
        case subsys::ADCS:
            sub_data = std::make_unique<AdcsData>(a_packet, a_reader);
            break;
        case subsys::EPS:
            sub_data = std::make_unique<EpsData>(a_packet, a_reader);
            break;
        case subsys::TEMP_SENSORS:
            sub_data = std::make_unique<TempData>(a_packet, a_reader);
            break;
        case subsys::SUN_SENSORS:
            sub_data = std::make_unique<SunData>(a_packet, a_reader);
            break;
        default:
            throw ProtocolException("Subsystem requested has no housekeeping data structure.");
    }
}

CommData::CommData(const csp::Packet& a_packet, const uint8_t** a_reader)
: ipAddress(Protocol::unpack<uint16_t>(a_packet, a_reader))
, port(Protocol::unpack<uint8_t>(a_packet, a_reader))
, packetSize(Protocol::unpack<uint64_t>(a_packet, a_reader))
{
}

AdcsData::AdcsData(const csp::Packet& a_packet, const uint8_t** a_reader)
: xrotationAngle(Protocol::unpack<double>(a_packet, a_reader))
, yrotationAngle(Protocol::unpack<double>(a_packet, a_reader))
, zrotationAngle(Protocol::unpack<double>(a_packet, a_reader))
{
}

EpsData::EpsData(const csp::Packet& a_packet, const uint8_t** a_reader)
: batteryPercentage(Protocol::unpack<double>(a_packet, a_reader))
, voltage(Protocol::unpack<double>(a_packet, a_reader))
, current(Protocol::unpack<double>(a_packet, a_reader))
{
}

TempData::TempData(const csp::Packet& a_packet, const uint8_t** a_reader)
: temperatue(Protocol::unpack<double>(a_packet, a_reader))
{
}

SunData::SunData(const csp::Packet& a_packet, const uint8_t** a_reader)
: xAxisAngle(Protocol::unpack<double>(a_packet, a_reader))
, yAxisAngle(Protocol::unpack<double>(a_packet, a_reader))
, zAxisAngle(Protocol::unpack<double>(a_packet, a_reader))
{
}

// temporary(?) default constructors

CommData::CommData(uint16_t a_ipAddress, uint8_t a_port, uint64_t a_packetSize) noexcept
: ipAddress(a_ipAddress)
, port(a_port)
, packetSize(a_packetSize)
{
}

} //sat