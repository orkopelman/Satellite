#ifndef MUHSAT_PROTOCOL_HPP
#define MUHSAT_PROTOCOL_HPP

#include <optional>
#include <vector>
#include <memory>

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

namespace sat {

class ProtocolException : public fsl::standard_exception {
    using standard_exception::standard_exception;
};

namespace prot {

// ProtocolCode will open each data segment of every packet :
// ProtocolCode (uint8_t) | SUB_PROTOCOL (type) | data (type) | [optional] ...

#define PROTOCOL_CODES(_)\
\
    /* General System */\
    _(PING, 0)          /* PING | SUBSYSTEM (uint8_t) | */ \
    _(PONG, 1)          /* PONG | SUBSYSTEM (uint8_t) | */ \
    _(PANIC, 2)         /* switch to safe mode immediatly */ \
\
    /* Uplink */\
/* // ! Commands sent together aren't guaranteed to be executed in the order they were packed ! */\
    _(COMMAND, 3)       /* COMMAND | validation_key (uint64_t) | \
                           COMMAND_CODE (uint8_t) | arg (uint8_t) \
                           [COMMAND_CODE (uint8_t)] ... */ \
    _(DELAYED_COMM, 4)  /* DELAYED_COMM | validation_key (uint64_t) | epoch_time_in_sec (uint64_t) | \
                           COMMAND_CODE (uint8_t) | arg (uint8_t) \
                           [COMMAND_CODE (uint8_t)] ... */ \
    _(HK_RECEIVED, 5)   /* HK_RECEIVED | SUBSYSTEM (uint8_t) | epoch_time_in_sec (uint64_t) */ \
\
    /* Downlink */\
    _(HK_PACKET, 6)     /* HK_PACKET | SUBSYSTEM (uint8_t) | \
                           sql_line (devided by collector's protocol) | [sql_line] ... */ \
    _(MODE_SWITCHED, 7) /* MODE_SWITCHED | MODE (uint8_t) | epoch_time_in_sec (uint64_t) */ \
    _(ANOMALY, 8)       /* ANOMALY | epoch_time_in_sec (uint64_t) | char[] | */ \
\
    /* Trackers */\
    _(EVENT_OCCURED, 9) /* EVENT_OCCURED | TRACKER (uint8_t) | EVENT (uint8_t) */ \
\
    /* Handlers */\
    _(HANDLE, 10)       /* HANDLE | SUBSYSTEM (uint8_t) | REQUEST (uint8_t) */\
\
    /* Flow Control */\
    _(REBOOT, 11)       /* REBOOT | SUBSYSTEM (uint8_t) | */\
    _(TURN_ON, 12)      /* TURN_ON | SUBSYSTEM (uint8_t) | */\
    _(TURN_OFF, 13)     /* TURN_OFF | SUBSYSTEM (uint8_t) | */

SPAM(PROTOCOL_CODES, ProtocolCodeT);

} // prot

class Protocol {
public:
    using CommandChain = std::vector<command::Request>;
    using HouseKeepingDataBlock = std::vector<std::shared_ptr<HouseKeepingData> >;
    using AnomalyReport = std::pair<std::string, uint64_t>;

public:
    Protocol() = default;
    Protocol(const Protocol& a_other) = default;
    Protocol& operator=(const Protocol& a_other) = default;
    ~Protocol() = default;

    /* Getters */
    static prot::ProtocolCodeT Type(const csp::Packet& a_packet);
    static std::optional<uint8_t> SubProtocol(const csp::Packet& a_packet);

    /* Packers */
    static void Ping(csp::Packet& a_packet, subsys::SubsystemsT a_destination);
    static void Pong(csp::Packet& a_packet, subsys::SubsystemsT a_destination);
    static void Panic(csp::Packet& a_packet);

    static void Command(csp::Packet& a_packet, command::Request a_command);
    static void Command(csp::Packet& a_packet, const CommandChain& a_commandChain);
    static void DelayCommand(csp::Packet& a_packet, command::Request a_command, uint64_t a_epochTimeInSec);
    static void DelayCommand(csp::Packet& a_packet, const CommandChain& a_commandChain, uint64_t a_epochTimeInSec);
    static void HousekeepingDataReceived(csp::Packet& a_packet, subsys::SubsystemsT a_subsystem, uint64_t a_timeStamp);

    static void PackHouseKeepingData(csp::Packet& a_packet, 
                                 subsys::SubsystemsT a_subsystem, 
                                 const HouseKeepingDataBlock& a_dataBlock);
    static void SwitchedMode(csp::Packet& a_packet, mode::ModeT a_toMode, uint64_t a_atEpoch = 0);
    static void Anomaly(csp::Packet& a_packet, const std::string& a_details);
    
    static void EventOccured(csp::Packet& a_packet, track::TrackerT a_tracker, event::EventT a_event);

    static void HandlerRequest(csp::Packet& a_packet, subsys::SubsystemsT a_handler, handle::HandlerRequestT a_request);

    static void Reboot(csp::Packet& a_packet, subsys::SubsystemsT a_destination);
    static void TurnOn(csp::Packet& a_packet, subsys::SubsystemsT a_destination);
    static void TurnOff(csp::Packet& a_packet, subsys::SubsystemsT a_destination);

    /* Unpackers */
    
    static std::unique_ptr<CommandChain> UnpackCommand(const csp::Packet& a_packet);
    static uint64_t UnpackDelay(const csp::Packet& a_packet);
    static uint64_t UnpackHKDReceivedTimeStamp(const csp::Packet& a_packet);

    static std::unique_ptr<HouseKeepingDataBlock> UnpackHouseKeepingData(const csp::Packet& a_packet);
    static uint64_t UnpackModeSwitchedTimeStamp(const csp::Packet& a_packet);
    static std::unique_ptr<AnomalyReport> UnpackAnomaly(const csp::Packet& a_packet);

    static event::EventT UnpackEvent(const csp::Packet& a_packet);
    static handle::HandlerRequestT UnpackHandlerRequest(const csp::Packet& a_packet);

    /* Checkers */
    static prot::ProtocolCodeT CheckProtocolValidity(uint8_t a_protocol); 
    static subsys::SubsystemsT CheckSubsystemValidity(uint8_t a_subsystem); 
    static mode::ModeT CheckModeValidity(uint8_t a_mode); 
    static track::TrackerT CheckTrackerValidity(uint8_t a_tracker); 
    static event::EventT CheckEventValidity(uint8_t a_event); 
    static handle::HandlerRequestT CheckRequestValidity(uint8_t a_request); 
    static command::CommandsT CheckCommandValidity(uint8_t a_command); 

    /* Utility */
    template<typename T>
    static void pack(csp::Packet& a_packet, T a_primitive);

    template<typename T>
    static T unpack(const csp::Packet& a_packet, const uint8_t** a_reader);

private:
    static void validateEmpty(const csp::Packet& a_packet);

    static uint8_t bytesRemain(const csp::Packet& a_packet, const uint8_t* a_reader);

    static void validateType(const csp::Packet& a_packet, prot::ProtocolCodeT a_type);

    static void packRequest(csp::Packet& a_packet, const command::Request& a_request);
    static void pack2EnumMessage(csp::Packet& a_packet, uint8_t a_firstEnum, uint8_t a_secondEnum);
};

} // sat

#include "inl/muhsat_protocol.hxx"

#endif /* MUHSAT_PROTOCOL_HPP */