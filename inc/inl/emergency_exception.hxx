#include "emergency_exception.hpp"
#include "csp_packet.hpp"
#include "muhsat_protocol.hpp"

namespace sat {

inline EmergencyException::EmergencyException(const std::string& a_message)
: m_message(a_message)
{
    csp::Packet panic;
    Protocol::Panic(panic);
    // send panic packet

}

inline const char* EmergencyException::what() const
{
    return m_message.c_str();
}


} //sat