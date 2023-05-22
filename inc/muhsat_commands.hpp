#ifndef MUHSAT_COMMANDS_HPP
#define MUHSAT_COMMANDS_HPP

#include <utility>
#include <cstdint>

#include "spam.h"

namespace sat {

namespace command {

#define COMMANDS(_)\
\
    /* General */\
    _(SWITCH_MODE, 0)          /* arg - MODE (uint8_t) */\
    _(REBOOT, 1)               /* arg - SUBSYSTEM (uint8_t) */\
    _(TURN_ON, 2)              /* arg - SUBSYSTEM (uint8_t) */\
    _(TURN_OFF, 3)             /* arg - SUBSYSTEM (uint8_t) */\
\
    /* HK Handlers */\
    _(EPS_HANDLER, 4)          /* arg - HANDLER_REQUEST (uint8_t) */\
    _(ADCS_HANDLER, 5)         /* arg - HANDLER_REQUEST (uint8_t) */\
    _(SENSORS_HANDLER, 6)      /* arg - HANDLER_REQUEST (uint8_t) */\
    _(COM_HANDLER, 7)          /* arg - HANDLER_REQUEST (uint8_t) */\
    _(ANT_HANDLER, 8)          /* arg - HANDLER_REQUEST (uint8_t) */

SPAM(COMMANDS, CommandsT)

using Request = std::pair<CommandsT, uint8_t>;

} // command

} // sat

#endif /* MUHSAT_COMMANDS_HPP */