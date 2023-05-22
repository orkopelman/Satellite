#ifndef CSP_SERVER_SOCKET_HPP
#define CSP_SERVER_SOCKET_HPP

#include <memory>
#include <optional>

#include "csp-all.hpp"

#include "csp_client_socket.hpp"

namespace sat {

namespace csp {

class ServerSocket {
public:
    ServerSocket(size_t a_backlog, uint8_t a_port = CSP_ANY);
    ServerSocket(const ServerSocket& a_other) = delete;
    ServerSocket& operator=(const ServerSocket& a_other) = delete;
    ~ServerSocket() = default;

    std::optional<std::unique_ptr<ClientSocket> > Accept(uint32_t a_waitFor = CSP_MAX_TIMEOUT) noexcept;

private:
    csp_socket_t m_socket;
};

} // csp

} // sat

#include "inl/csp_server_socket.hxx"

#endif /* CSP_SERVER_SOCKET_HPP */