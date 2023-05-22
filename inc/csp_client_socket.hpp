#ifndef CSP_CLIENT_SOCKET_HPP
#define CSP_CLIENT_SOCKET_HPP

#include <memory>
#include <optional>
#include <utility>

#include "csp-all.hpp"

#include "csp_packet.hpp"
#include "fsl_exception.hpp"

namespace sat {

namespace csp {

class ClientSocketException : public fsl::standard_exception {
    using fsl::standard_exception::standard_exception;
};

class ClientSocket {
public:
    using destinationPort = int;
    using receivedMessage = std::pair<std::unique_ptr<Packet>, destinationPort>;

public:
    ClientSocket(uint16_t a_destAddress,
                 uint8_t a_destPort,
                 uint8_t a_priority = CSP_PRIO_NORM,
                 uint32_t a_waitFor = CSP_MAX_TIMEOUT,
                 uint32_t a_options = CSP_O_NONE);
    explicit ClientSocket(csp_conn_t* a_connection);
    ClientSocket(const ClientSocket& a_other) = delete;
    ClientSocket& operator=(const ClientSocket& a_other) = delete;
    ~ClientSocket();

    std::optional<receivedMessage> Read(uint32_t a_waitFor = CSP_MAX_TIMEOUT) noexcept;
    void Send(std::unique_ptr<Packet> a_packet) noexcept;

private:
    csp_conn_t* m_connection;
};

} // csp

} // sat

#include "inl/csp_client_socket.hxx"

#endif /* CSP_CLIENT_SOCKET_HPP */