#ifndef FSL_EXCEPTION_HXX
#define FSL_EXCEPTION_HXX

#include <exception>
#include <string>

namespace fsl {

inline standard_exception::standard_exception(const std::string& a_error)
: m_what(a_error)
{}

inline const char* standard_exception::what() const throw()
{
    return m_what.c_str();
}

inline std::string what(const std::exception_ptr& a_eptr)
{
    if (!a_eptr) { throw std::bad_exception(); }

    try { 
        std::rethrow_exception(a_eptr); 
    } catch (const std::exception &e) { 
        return e.what(); 
    } catch (const std::string &e) { 
        return e;
    } catch (const char *e) { 
        return e; 
    } catch (...) { 
        return "Unknown Error."; 
    }
}

} // fsl

#endif /* FSL_EXCEPTION_HXX */