#ifndef CSP_SERVER_SOCKET_HXX
#define CSP_SERVER_SOCKET_HXX

#include <memory>
#include <optional>
 
#include "csp_client_socket.hpp"

namespace sat {

namespace csp {

inline ServerSocket::ServerSocket(size_t a_backlog, uint8_t a_port)
: m_socket()
{
    ::memset(&m_socket, 0, sizeof(m_socket));
    ::csp_bind(&m_socket, a_port);
    ::csp_listen(&m_socket, a_backlog);
}

inline std::optional<std::unique_ptr<ClientSocket> > ServerSocket::Accept(uint32_t a_waitFor) noexcept
{
    csp_conn_t* acceptedClient = ::csp_accept(&m_socket, a_waitFor);
    if (acceptedClient == nullptr) {
        return {};
    } else {
        return std::make_unique<ClientSocket>(acceptedClient);
    }
}

} // csp

} // sat

#endif /* CSP_SERVER_SOCKET_HXX */