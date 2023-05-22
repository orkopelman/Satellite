#ifndef TRCKER_SUN_HXX
#define TRCKER_SUN_HXX

#include "tracker_sun.hpp"

namespace sat {

namespace details {

inline const char* SunTracker::TableName() const noexcept 
{
    return sun_table;
}

inline std::string SunTracker::CollectData() const noexcept 
{
    return m_db.Read(TableName());
}

inline subsys::SubsystemsT SunTracker::SubSys() const noexcept
{
    return subsys::SUN_SENSORS;
}

inline size_t SunTracker::IntermissionLength() const noexcept 
{
    return; /// configuration file!
}

} // details

} // sat

#endif //TRCKER_SUN_HXX
