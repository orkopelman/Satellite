
#include <thread>

#include "csp_boot.hpp"
#include "csp-all.hpp"

namespace sat {

namespace csp {

namespace details {

void Router() 
{
    while (1) {
        csp_route_work();
    }
}

} // details

void BootCSP() {
    /* Init CSP */
    csp_init();
    /* Init CSP router */
    std::thread router{details::Router};
    router.detach();
}

} // csp

} // sat
