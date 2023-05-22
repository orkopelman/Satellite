#ifndef MUHSAT_PROTOCOL_HXX
#define MUHSAT_PROTOCOL_HXX

#include <optional>
#include <vector>
#include <memory>
#include <algorithm>

#include "spam.h"
#include "csp_packet.hpp"
#include "muhsat_router.hpp"
#include "muhsat_commands.hpp"
#include "muhsat_modes.hpp"
#include "housekeeping_data.hpp"
#include "interface_tracker.hpp"
#include "interface_handler.hpp"
#include "interface_trackable_event.hpp"
#include "fsl_exception.hpp"

#include "fsl_endians.hpp"

namespace sat {

namespace detail {

inline void CheckCommandChain(const Protocol::CommandChain& a_commandChain) 
{
    if (a_commandChain.empty()) {
        throw ProtocolException("Can't Pack an Empty Command Chain.");
    }
}

} // detail

/* Getters */

inline prot::ProtocolCodeT Protocol::Type(const csp::Packet& a_packet)
{
    return CheckProtocolValidity(*a_packet.Data());
}

inline std::optional<uint8_t> Protocol::SubProtocol(const csp::Packet& a_packet)
{
    prot::ProtocolCodeT type = Type(a_packet);
    switch(type) {
    case prot::PING:
    case prot::PONG:
    case prot::HK_RECEIVED:
    case prot::HK_PACKET:
    case prot::HANDLE:
    case prot::REBOOT:
    case prot::TURN_ON:
    case prot::TURN_OFF:
        return CheckSubsystemValidity(a_packet.Data()[1]);
    case prot::MODE_SWITCHED:
        return CheckModeValidity(a_packet.Data()[1]);
    case prot::EVENT_OCCURED:
        return CheckTrackerValidity(a_packet.Data()[1]);
    default:
        return {};
    }
}

/* Packers */

inline void Protocol::Ping(csp::Packet& a_packet, subsys::SubsystemsT a_destination)
{
    pack2EnumMessage(a_packet, prot::PING, CheckSubsystemValidity(a_destination));
}

inline void Protocol::Pong(csp::Packet& a_packet, subsys::SubsystemsT a_destination)
{
    pack2EnumMessage(a_packet, prot::PONG, CheckSubsystemValidity(a_destination));
}

inline void Protocol::Panic(csp::Packet& a_packet)
{
    validateEmpty(a_packet);
    pack<uint8_t>(a_packet, prot::PANIC);
}

inline void Protocol::Command(csp::Packet& a_packet, command::Request a_command)
{
    validateEmpty(a_packet);
    uint64_t validationKey = 0; // TODO from configuration
    pack<uint8_t>(a_packet, prot::COMMAND);
    pack<uint64_t>(a_packet, validationKey);
    packRequest(a_packet, a_command);
}

inline void Protocol::Command(csp::Packet& a_packet, const CommandChain& a_commandChain)
{
    detail::CheckCommandChain(a_commandChain);
    Command(a_packet, a_commandChain[0]);
    std::for_each(++a_commandChain.begin(), a_commandChain.end(), [&a_packet](const command::Request& a_command){
        packRequest(a_packet, a_command);
    });
}

inline void Protocol::DelayCommand(csp::Packet& a_packet, command::Request a_command, uint64_t a_epochTimeInSec)
{
    validateEmpty(a_packet);
    uint64_t validationKey = 0; // TODO from configuration
    pack<uint8_t>(a_packet, prot::DELAYED_COMM);
    pack<uint64_t>(a_packet, validationKey);
    pack<uint64_t>(a_packet, a_epochTimeInSec);
    packRequest(a_packet, a_command);
}

inline void Protocol::DelayCommand(csp::Packet& a_packet, const CommandChain& a_commandChain, uint64_t a_epochTimeInSec)
{
    detail::CheckCommandChain(a_commandChain);
    DelayCommand(a_packet, a_commandChain[0], a_epochTimeInSec);
    std::for_each(++a_commandChain.begin(), a_commandChain.end(), [&a_packet](const command::Request& a_command){
        packRequest(a_packet, a_command);
    });
}

inline void Protocol::HousekeepingDataReceived(csp::Packet& a_packet, subsys::SubsystemsT a_subsystem, uint64_t a_timeStamp)
{
    validateEmpty(a_packet);
    pack<uint8_t>(a_packet, prot::HK_RECEIVED);
    pack<uint8_t>(a_packet, a_subsystem);
    pack<uint64_t>(a_packet, a_timeStamp);
}

inline void Protocol::PackHouseKeepingData(csp::Packet& a_packet, 
                                       subsys::SubsystemsT a_subsystem, 
                                       const HouseKeepingDataBlock& a_dataBlock)
{
    pack2EnumMessage(a_packet, prot::HK_PACKET, a_subsystem);
    std::for_each(a_dataBlock.begin(), a_dataBlock.end(), [&a_packet](std::shared_ptr<sat::HouseKeepingData> a_hkData){
        a_hkData->Pack(a_packet);
    });
}

inline void Protocol::SwitchedMode(csp::Packet& a_packet, mode::ModeT a_toMode, uint64_t a_atEpoch)
{
    pack2EnumMessage(a_packet, prot::MODE_SWITCHED, a_toMode);
    pack<uint64_t>(a_packet, a_atEpoch);
}

inline void Protocol::Anomaly(csp::Packet& a_packet, const std::string& a_details)
{
    validateEmpty(a_packet);
    pack<uint8_t>(a_packet, prot::ANOMALY);
    pack<uint64_t>(a_packet, time(nullptr));
    a_packet.Append(a_details.c_str(), a_details.size());
}

inline void Protocol::EventOccured(csp::Packet& a_packet, track::TrackerT a_tracker, event::EventT a_event)
{
    pack2EnumMessage(a_packet, prot::EVENT_OCCURED, a_tracker);
    pack<uint8_t>(a_packet, a_event);
}

inline void Protocol::HandlerRequest(csp::Packet& a_packet, subsys::SubsystemsT a_handler, handle::HandlerRequestT a_request)
{
    pack2EnumMessage(a_packet, prot::HANDLE, a_handler);
    pack<uint8_t>(a_packet, a_request);
}

inline void Protocol::Reboot(csp::Packet& a_packet, subsys::SubsystemsT a_destination)
{
    pack2EnumMessage(a_packet, prot::REBOOT, a_destination);
}

inline void Protocol::TurnOn(csp::Packet& a_packet, subsys::SubsystemsT a_destination)
{
    pack2EnumMessage(a_packet, prot::TURN_ON, a_destination);
}

inline void Protocol::TurnOff(csp::Packet& a_packet, subsys::SubsystemsT a_destination)
{
    pack2EnumMessage(a_packet, prot::TURN_OFF, a_destination);
}

/* Unpackers */

inline std::unique_ptr<Protocol::CommandChain> Protocol::UnpackCommand(const csp::Packet& a_packet)
{
    const uint8_t* reader = a_packet.Data() + sizeof(uint8_t) + sizeof(uint64_t);
    switch (Type(a_packet)) {
        case prot::COMMAND:
            break;
        case prot::DELAYED_COMM:
            reader += sizeof(uint64_t);
            break;
        default:
            validateType(a_packet, prot::COMMAND);
    }
    auto chain = std::make_unique<CommandChain>();
    while (bytesRemain(a_packet, reader)) {
        sat::command::CommandsT command = CheckCommandValidity(unpack<uint8_t>(a_packet, &reader));
        chain->emplace_back(command, unpack<uint8_t>(a_packet, &reader));
    }
    return chain;
}

inline uint64_t Protocol::UnpackDelay(const csp::Packet& a_packet)
{
    validateType(a_packet, prot::DELAYED_COMM);
    const uint8_t* reader = a_packet.Data() + sizeof(uint8_t) + sizeof(uint64_t);
    return unpack<uint64_t>(a_packet, &reader);
}

inline uint64_t Protocol::UnpackHKDReceivedTimeStamp(const csp::Packet& a_packet)
{
    validateType(a_packet, prot::HK_RECEIVED);
    const uint8_t* reader = a_packet.Data() + sizeof(uint8_t) + sizeof(uint8_t);
    return unpack<uint64_t>(a_packet, &reader);
}

inline std::unique_ptr<Protocol::HouseKeepingDataBlock> Protocol::UnpackHouseKeepingData(const csp::Packet& a_packet)
{
    validateType(a_packet, prot::HK_PACKET);
    std::unique_ptr<HouseKeepingDataBlock> block = std::make_unique<HouseKeepingDataBlock>();
    auto subSystem = Protocol::SubProtocol(a_packet);
    if (!subSystem.has_value()) {
        throw ProtocolException("Message corrupted. House Keeping Data packet lacks subsystem.");
    }
    subsys::SubsystemsT houseKeepingSystem = static_cast<subsys::SubsystemsT>(Protocol::SubProtocol(a_packet).value());
    const uint8_t* reader = a_packet.Data() + sizeof(uint8_t) + sizeof(uint8_t);
    while (bytesRemain(a_packet, reader)) {
        block->push_back(std::make_shared<HouseKeepingData>(a_packet, &reader, houseKeepingSystem));
    }
    return block;
}

inline uint64_t Protocol::UnpackModeSwitchedTimeStamp(const csp::Packet& a_packet)
{
    validateType(a_packet, prot::MODE_SWITCHED);
    const uint8_t* reader = a_packet.Data() + sizeof(uint8_t) + sizeof(uint8_t);
    return unpack<uint64_t>(a_packet, &reader);
}

inline std::unique_ptr<Protocol::AnomalyReport> Protocol::UnpackAnomaly(const csp::Packet& a_packet)
{
    validateType(a_packet, prot::ANOMALY);
    constexpr uint16_t stringStart = sizeof(uint8_t) + sizeof(uint64_t);
    if (a_packet.Length() < stringStart) {
        throw ProtocolException("Message corrupted.");
    }
    return std::make_unique<AnomalyReport>(
        std::string(reinterpret_cast<const char*>(a_packet.Data() + stringStart), a_packet.Length() - stringStart), 
        fsl::TranslateToHostEndian<uint64_t>(*reinterpret_cast<const uint64_t*>(a_packet.Data() + 1)));
}

inline event::EventT Protocol::UnpackEvent(const csp::Packet& a_packet)
{
    validateType(a_packet, prot::EVENT_OCCURED);
    const uint8_t* reader = a_packet.Data() + sizeof(uint8_t) + sizeof(uint8_t);
    return CheckEventValidity(unpack<uint8_t>(a_packet, &reader));
}

inline handle::HandlerRequestT Protocol::UnpackHandlerRequest(const csp::Packet& a_packet)
{
    validateType(a_packet, prot::HANDLE);
    const uint8_t* reader = a_packet.Data() + sizeof(uint8_t) + sizeof(uint8_t);
    return CheckRequestValidity(unpack<uint8_t>(a_packet, &reader));
}

/* Checkers */

inline prot::ProtocolCodeT Protocol::CheckProtocolValidity(uint8_t a_protocol) 
{
    if (!BON(prot::ProtocolCodeT, isValid)(static_cast<prot::ProtocolCodeT>(a_protocol))) {
        throw ProtocolException("Protocol Code is invalid.");
    }
    return static_cast<prot::ProtocolCodeT>(a_protocol);
}

inline subsys::SubsystemsT Protocol::CheckSubsystemValidity(uint8_t a_subsystem)
{
    if (!BON(subsys::SubsystemsT, isValid)(static_cast<subsys::SubsystemsT>(a_subsystem))) {
        throw ProtocolException("Subsystem Code is invalid.");
    }
    return static_cast<subsys::SubsystemsT>(a_subsystem);
}

inline mode::ModeT Protocol::CheckModeValidity(uint8_t a_mode)
{
    if (!BON(mode::ModeT, isValid)(static_cast<mode::ModeT>(a_mode))) {
        throw ProtocolException("Mode Code is invalid.");
    }
    return static_cast<mode::ModeT>(a_mode);
}

inline track::TrackerT Protocol::CheckTrackerValidity(uint8_t a_tracker)
{
    if (!BON(track::TrackerT, isValid)(static_cast<track::TrackerT>(a_tracker))) {
        throw ProtocolException("Tracker Code is invalid.");
    }
    return static_cast<track::TrackerT>(a_tracker);
}

inline event::EventT Protocol::CheckEventValidity(uint8_t a_event)
{
    if (!BON(event::EventT, isValid)(static_cast<event::EventT>(a_event))) {
        throw ProtocolException("Event Code is invalid.");
    }
    return static_cast<event::EventT>(a_event);
}

inline handle::HandlerRequestT Protocol::CheckRequestValidity(uint8_t a_request)
{
    if (!BON(handle::HandlerRequestT, isValid)(static_cast<handle::HandlerRequestT>(a_request))) {
        throw ProtocolException("Request Code is invalid.");
    }
    return static_cast<handle::HandlerRequestT>(a_request);
}

inline command::CommandsT Protocol::CheckCommandValidity(uint8_t a_command)
{
    if (!BON(command::CommandsT, isValid)(static_cast<command::CommandsT>(a_command))) {
        throw ProtocolException("Command Code is invalid.");
    }
    return static_cast<command::CommandsT>(a_command);
}

/* Utility */

template<typename T>
inline void Protocol::pack(csp::Packet& a_packet, T a_primitive)
{
    a_packet.Append(fsl::TranslateToNetEndian<T>(a_primitive), sizeof(T));
}

template<typename T>
inline T Protocol::unpack(const csp::Packet& a_packet, const uint8_t** a_reader)
{
    if (a_reader == nullptr) {
        throw ProtocolException("Can't read message. Reading pointr is null.");
    }
    if (bytesRemain(a_packet, *a_reader) < sizeof(T)) {
        throw ProtocolException("Message Corrupted - unpack request exeeding packet length.");
    }
    const T* itemPtr = reinterpret_cast<const T*>(*a_reader);
    *a_reader += sizeof(T);
    return fsl::TranslateToHostEndian<T>(*itemPtr);
}

/* Private */

inline void Protocol::validateEmpty(const csp::Packet& a_packet)
{
    if (a_packet.Length() != 0) {
        throw ProtocolException("Can't pack an already packed message.");
    }
}

inline uint8_t Protocol::bytesRemain(const csp::Packet& a_packet, const uint8_t* a_reader)
{
    if (a_reader == nullptr) {
        throw ProtocolException("Can't calculate distance from nullptr.");
    }
    int16_t remain = (a_packet.Data() + a_packet.Length()) - a_reader;
    return remain > 0 ? remain : 0;
}

inline void Protocol::validateType(const csp::Packet& a_packet, prot::ProtocolCodeT a_type)
{
    if (Type(a_packet) != a_type) {
        std::string errMessage{"Can't unpack message of type "};
        errMessage += BON(prot::ProtocolCodeT, toString)(Type(a_packet));
        errMessage += " as a ";
        errMessage += BON(prot::ProtocolCodeT, toString)(a_type);
        throw ProtocolException(errMessage);
    }
}

inline void Protocol::packRequest(csp::Packet& a_packet, const command::Request& a_request)
{
    pack<uint8_t>(a_packet, a_request.first);
    pack<uint8_t>(a_packet, a_request.second);
}

inline void Protocol::pack2EnumMessage(csp::Packet& a_packet, uint8_t a_firstEnum, uint8_t a_secondEnum)
{
    validateEmpty(a_packet);
    pack<uint8_t>(a_packet, a_firstEnum);
    pack<uint8_t>(a_packet, a_secondEnum);
}

} // sat

#endif /* MUHSAT_PROTOCOL_HXX */