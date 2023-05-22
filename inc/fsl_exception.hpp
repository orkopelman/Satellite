#ifndef FSL_EXCEPTION_HPP
#define FSL_EXCEPTION_HPP

#include <exception>
#include <string>

namespace fsl {

class standard_exception: public std::exception {
public:
    standard_exception(const std::string& a_error);
    virtual const char* what() const throw();

private:
    std::string m_what;
};

std::string what(const std::exception_ptr& a_eptr = std::current_exception());

} // fsl

#include "inl/fsl_exception.hxx"

#endif /* FSL_EXCEPTION_HPP */