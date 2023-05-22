#ifndef INTERFACE_HANDLER_HPP
#define INTERFACE_HANDLER_HPP

#include "spam.h"

namespace sat {

namespace handle {

#define HANDLER_REQUEST(_) \
\
    /* General Requests */ \
    _(LOW_POWER, 0) \
    _(MID_POWER, 1) \
    _(HIGH_POWER, 2) \
    /* Frequency */ \
    _(NORMAL_FREQ, 3) \
    _(REDUCED_FREQ, 4) \
    _(NO_FREQ, 5)           /* turned off */

SPAM(HANDLER_REQUEST, HandlerRequestT);

} // handle

} // sat

#endif /* INTERFACE_HANDLER_HPP */