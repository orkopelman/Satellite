#ifndef EMERGENCY_EXCEPTION_HPP
#define EMERGENCY_EXCEPTION_HPP

#include <string>

namespace sat {

class EmergencyException {
public:
    explicit EmergencyException(const std::string& a_message);

    const char* what() const;

private:
    std::string m_message;
};

} //sat

#include "inl/emergency_exception.hxx"

#endif