#ifndef HOUSEKEEPING_DATA_HPP
#define HOUSEKEEPING_DATA_HPP

#include <memory>
#include <ctime>
#include <string>
#include <vector>

#include "muhsat_router.hpp" // subsys::SubsystemsT
#include "csp_packet.hpp"

namespace sat {

struct SubSystemData;

struct HouseKeepingData {
    explicit HouseKeepingData(std::unique_ptr<SubSystemData> a_subData);
    HouseKeepingData(subsys::SubsystemsT a_sybsystem, std::string& a_readData);
    HouseKeepingData(const csp::Packet& a_packet, const uint8_t** a_reader, subsys::SubsystemsT a_HKsubsys);

    void Pack(csp::Packet& a_packet) const;

    uint64_t curTime;
    std::unique_ptr<SubSystemData> sub_data;
};

struct SubSystemData {
    virtual ~SubSystemData() = default;

    virtual void Pack(csp::Packet& a_packet) const = 0;

protected:
    SubSystemData() = default;
    SubSystemData(const SubSystemData& a_other) = default;
    SubSystemData& operator=(const SubSystemData& a_other) = default;
};

struct CommData: public SubSystemData {
    uint16_t ipAddress;
    uint8_t port;
    uint64_t packetSize;

    void Pack(csp::Packet& a_packet) const override;

    CommData() = default;
    CommData(const csp::Packet& a_packet, const uint8_t** a_reader);

    // I built this one for tests - can be removed if isn't in use, or format should be extended to other structs
    CommData(uint16_t a_ipAddress, uint8_t a_port, uint64_t a_packetSize) noexcept;
};

struct AdcsData: public SubSystemData {
    double xrotationAngle;
    double yrotationAngle;
    double zrotationAngle;

    void Pack(csp::Packet& a_packet) const override;

    AdcsData() = default;
    AdcsData(const csp::Packet& a_packet, const uint8_t** a_reader);
};

struct EpsData: public SubSystemData {
    double batteryPercentage;
    double voltage;
    double current;

    void Pack(csp::Packet& a_packet) const override;

    EpsData() = default;
    EpsData(const csp::Packet& a_packet, const uint8_t** a_reader);
};

struct TempData: public SubSystemData {
    double temperatue;

    void Pack(csp::Packet& a_packet) const override;

    TempData() = default;
    TempData(const csp::Packet& a_packet, const uint8_t** a_reader);
};

struct SunData: public SubSystemData {
    double xAxisAngle;
    double yAxisAngle;
    double zAxisAngle;

    void Pack(csp::Packet& a_packet) const override;

    SunData() = default;
    SunData(const csp::Packet& a_packet, const uint8_t** a_reader);
};

// Number of subsystems' members and build functions.
namespace details {

constexpr uint8_t comm_members = 4;
constexpr uint8_t adcs_members = 4;
constexpr uint8_t eps_members = 4;
constexpr uint8_t temp_members = 2;
constexpr uint8_t sun_members = 4;


uint8_t SubSystemMembers(subsys::SubsystemsT a_subsystem);
std::unique_ptr<CommData> BuildComData(std::vector<std::string>& a_membersData);
std::unique_ptr<AdcsData> BuildAdcsData(std::vector<std::string>& a_membersData);
std::unique_ptr<EpsData> BuildEpsData(std::vector<std::string>& a_membersData);
std::unique_ptr<TempData> BuildTempData(std::vector<std::string>& a_membersData);
std::unique_ptr<SunData> BuildSunData(std::vector<std::string>& a_membersData);
std::unique_ptr<SubSystemData> BuildSubData(subsys::SubsystemsT a_subsystem, std::vector<std::string>& a_membersData);

} //details

} //sat

#endif