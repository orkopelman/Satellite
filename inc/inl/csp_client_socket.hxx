#ifndef CSP_CLIENT_SOCKET_HXX
#define CSP_CLIENT_SOCKET_HXX

#include <memory>
#include <optional>
#include <utility>

#include "csp-all.hpp"

#include "csp_packet.hpp"

namespace sat {

namespace csp {

namespace details {

static csp_conn_t* CheckConnection(csp_conn_t* a_connection)
{
    if (a_connection == nullptr) {
        throw ClientSocketException("Connection Failed.");
    }
    return a_connection;
}

} // details

inline ClientSocket::ClientSocket(uint16_t a_destAddress,
                                  uint8_t a_destPort,
                                  uint8_t a_priority,
                                  uint32_t a_waitFor,
                                  uint32_t a_options)
: m_connection(details::CheckConnection(csp_connect(a_priority, a_destAddress, a_destPort, a_waitFor, a_options)))
{}

inline ClientSocket::ClientSocket(csp_conn_t* a_connection)
: m_connection(details::CheckConnection(a_connection))
{}

inline ClientSocket::~ClientSocket()
{
    ::csp_close(m_connection);
}

inline std::optional<ClientSocket::receivedMessage> ClientSocket::Read(uint32_t a_waitFor) noexcept
{
    csp_packet_t* receivedPacket = ::csp_read(m_connection, a_waitFor);
    if (receivedPacket == nullptr) {
        return {};
    } else {
        return receivedMessage(std::make_unique<Packet>(receivedPacket),
                               ::csp_conn_dport(m_connection));
    }
}

inline void ClientSocket::Send(std::unique_ptr<Packet> a_packet) noexcept
{
    ::csp_send(m_connection, a_packet->m_packet);
    a_packet->m_sent = true;
}

} // csp

} // sat

#endif /* CSP_CLIENT_SOCKET_HXX */