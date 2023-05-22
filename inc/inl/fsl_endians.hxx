#ifndef FSL_ENDIANS_HXX
#define FSL_ENDIANS_HXX

#define __OPTIMIZE__

#include <stdexcept>
#include <netinet/in.h>

namespace fsl {

namespace details {

inline uint64_t htonll(uint64_t a_value) noexcept
{
    return htobe64(a_value);
}

inline uint64_t ntohll(uint64_t a_value) noexcept
{
    return be64toh(a_value);
}

} // details

template<typename T>
T TranslateToNetEndian(T a_value)
{
    #  if __BYTE_ORDER == __LITTLE_ENDIAN
        switch (sizeof(a_value)) {
        case 1: 
            break;
        case 2:
            return static_cast<T>(::htons(a_value));
        case 4:
            return static_cast<T>(::htonl(a_value));
        case 8:
            return static_cast<T>(details::htonll(a_value));
        default:
            throw std::invalid_argument("Can't Translate Unprimitive Types.");
        }
    # endif
    return a_value;
}

inline double TranslateToNetEndian(double a_value)
{
    return a_value;
}

template<typename T>
T TranslateToHostEndian(T a_value)
{
    #  if __BYTE_ORDER == __LITTLE_ENDIAN
        switch (sizeof(a_value)) {
        case 1:
            break;
        case 2:
            return static_cast<T>(::ntohs(a_value));
        case 4:
            return static_cast<T>(::ntohl(a_value));
        case 8:
            return static_cast<T>(details::ntohll(a_value));
        default:
            throw std::invalid_argument("Can't Translate Unprimitive Types.");
        }
    # endif
    return a_value;
}

inline double TranslateToHostEndian(double a_value)
{
    return a_value;
}

template<typename T>
void ArrayToNetEndian(void* a_array, size_t a_arraySize)
{
    #  if __BYTE_ORDER == __LITTLE_ENDIAN
        size_t unitSize = sizeof(T);
        if (a_array == nullptr) {
            throw std::invalid_argument("Pointer to array can't be null.");
        }
        if (unitSize > 1) {
            for (size_t i = 0; i < a_arraySize; i += unitSize) {
                T translated = TranslateToNetEndian<T>(*(reinterpret_cast<T*>(&static_cast<char*>(a_array)[i])));
                ::memcpy((&static_cast<char*>(a_array)[i]), &translated, unitSize);
            }
        }
    # endif
}

template<typename T>
void ArrayToHostEndian(void* a_array, size_t a_arraySize)
{
    #  if __BYTE_ORDER == __LITTLE_ENDIAN
        size_t unitSize = sizeof(T);
        if (a_array == nullptr) {
            throw std::invalid_argument("Pointer to array can't be null.");
        }
        if (unitSize > 1) {
            for (size_t i = 0; i < a_arraySize; i += unitSize) {
                T translated = TranslateToHostEndian<T>(*(reinterpret_cast<T*>(&static_cast<char*>(a_array)[i])));
                ::memcpy((&static_cast<char*>(a_array)[i]), &translated, unitSize);
            }
        }
    # endif
}

inline bool IsNetEndian() noexcept
{
    # if __BYTE_ORDER == __BIG_ENDIAN
        return true;
    # endif
    #  if __BYTE_ORDER == __LITTLE_ENDIAN
        return false;
    # endif
}

} // fsl

#endif /* FSL_ENDIANS_HXX */