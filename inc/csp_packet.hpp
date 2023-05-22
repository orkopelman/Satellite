#ifndef CSP_PACKET_HPP
#define CSP_PACKET_HPP

#include <atomic>

#include "csp-all.hpp"
#include "fsl_exception.hpp"

namespace sat {

namespace csp {

class packetException : public fsl::standard_exception {
    using fsl::standard_exception::standard_exception;
};

constexpr size_t MTUinBytes = 256;

class Packet {
public:
    explicit Packet(size_t a_dataSizeInBytes = MTUinBytes);
    explicit Packet(csp_packet_t* a_packet); // read-only received packet
    Packet(const Packet& a_other) = delete;
    Packet& operator=(const Packet& a_other) = delete;
    ~Packet();

    const uint8_t* Data() const noexcept;
    uint16_t Length() const noexcept;

    template<typename T>
    void Append(const T& a_data, size_t a_sizeInBytes);

    template<typename T>
    void Append(const T* a_data, size_t a_sizeInBytes);

    void ServicePacket() noexcept;

private:
    friend class ClientSocket;

    csp_packet_t* m_packet;
    const bool m_readOnly;
    std::atomic_bool m_sent;
};

} // csp

} // sat

#include "inl/csp_packet.hxx"

#endif /* CSP_PACKET_HPP */