#ifndef MUHSAT_ROUTER_HPP
#define MUHSAT_ROUTER_HPP

#include "csp_server_socket.hpp"
#include "spam.h"

namespace sat {

namespace subsys {

#define SUBSYSTEMS(_)\
\
    /* General System */\
    _(FLIGHT_PLANNER, 0)\
    _(DOWNLINK, 1)\
\
    /* HK Subsystems */\
    _(EPS, 2)\
    _(ADCS, 3)\
    _(SUN_SENSORS, 4)\
    _(TEMP_SENSORS, 5)\
    _(COM, 6)\
    _(ANT, 7)\
\
    /* Payloads */\
    _(PAYLOADS, 14)\
\
    /* Broadcast */\
    _(BROADCAST, 15)

SPAM(SUBSYSTEMS, SubsystemsT)

} // subsys

class Router {
public:
    Router();
    Router(const Router& a_other) = delete;
    Router operator=(const Router& a_other) = delete;
    ~Router() = default;

private:
    csp::ServerSocket m_serverSocket;
};

} // sat

#endif /* MUHSAT_ROUTER_HPP */