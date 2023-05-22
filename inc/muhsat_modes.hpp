#ifndef MUHSAT_MODES_HPP
#define MUHSAT_MODES_HPP

#include "spam.h"

namespace sat {

namespace mode {

#define MODE(_)\
    _(INIT, 0)\
    _(SAFE, 1)\
    _(DOWNLINK, 2)\
    _(CRITICAL, 3)\
    _(ECLIPSE, 4)\
    _(MISSION, 5)

SPAM(MODE, ModeT);

} // mode

} // sat

#endif /* MUHSAT_MODES_HPP */