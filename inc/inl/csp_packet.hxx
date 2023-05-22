#ifndef CSP_PACKET_HXX
#define CSP_PACKET_HXX

#include <cstring> /* size_t, memcpy */

#include "csp-all.hpp"

namespace sat {

namespace csp {

namespace details {

static size_t CheckMTU(size_t a_dataSizeInBytes)
{
    if (a_dataSizeInBytes > MTUinBytes) {
        throw packetException("Buffer Overflow.");
    }
    return a_dataSizeInBytes;
}

static csp_packet_t* CheckNull(void* a_potentialBuffer)
{
    if (a_potentialBuffer == nullptr) {
        throw packetException("Couldn't allocate buffer.");
    }
    return static_cast<csp_packet_t*>(a_potentialBuffer);
}

} // details

inline Packet::Packet(size_t a_dataSizeInBytes)
: m_packet(details::CheckNull(csp_buffer_get(details::CheckMTU(a_dataSizeInBytes))))
, m_readOnly(false)
, m_sent(false)
{
    m_packet->length = 0;
}

inline Packet::Packet(csp_packet_t* a_packet)
: m_packet(details::CheckNull(a_packet))
, m_readOnly(true)
, m_sent(false)
{}

inline Packet::~Packet()
{
    if (!m_sent) {
        ::csp_buffer_free(m_packet);
    }
}

inline const uint8_t* Packet::Data() const noexcept
{
    return m_packet->data;
}

inline uint16_t Packet::Length() const noexcept
{
    return m_packet->length;
}

template<typename T>
void Packet::Append(const T& a_data, size_t a_sizeInBytes)
{
    if (m_readOnly) {
        throw packetException("Can't write to a read only packet.");
    }
    if (m_sent) {
        throw packetException("Can't write to packet after it had been sent.");
    }
    details::CheckMTU(a_sizeInBytes + m_packet->length);
    ::memcpy(m_packet->data + m_packet->length, &a_data, a_sizeInBytes);
    m_packet->length += a_sizeInBytes;
}

template<typename T>
void Packet::Append(const T* a_data, size_t a_sizeInBytes)
{
    if (m_readOnly) {
        throw packetException("Can't write to a read only packet.");
    }
    if (m_sent) {
        throw packetException("Can't write to packet after it had been sent.");
    }
    details::CheckMTU(a_sizeInBytes + m_packet->length);
    ::memcpy(m_packet->data + m_packet->length, a_data, a_sizeInBytes);
    m_packet->length += a_sizeInBytes;
}

inline void Packet::ServicePacket() noexcept
{
    csp_service_handler(m_packet);
}

} // csp

} // sat

#endif /* CSP_PACKET_HXX */