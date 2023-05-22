#ifndef EVENT_SUN_HXX
#define EVENT_SUN_HXX

#include <memory>

#include "muhsat_protocol.hpp"
#include "housekeeping_data.hpp"

namespace sat {

namespace details {

double AngleCalc(double _a, double _b, double _c) 
{
    return (_a + _b + _c) / 3;
}

double SunAngle(std::shared_ptr<csp::Packet> a_currentReading)
{
    std::unique_ptr<SunData> sunDataPtr(dynamic_cast<SunData*>(IntermediateTrackableEvent::SubData(a_currentReading).release()));
    return AngleCalc(sunDataPtr->xAxisAngle, sunDataPtr->yAxisAngle, sunDataPtr->zAxisAngle);
}

} //details

inline SolarEclipse::SolarEclipse(double a_criteria)
: m_criteria(a_criteria)
{}

inline PartialSolarEclipse::PartialSolarEclipse(double a_criteriaFrom, double a_criteriaUntil)
: m_criteriaFrom(a_criteriaFrom)
, m_criteriaUntil(a_criteriaUntil)
{}

inline FullSunExposure::FullSunExposure(double a_criteria)
: m_criteria(a_criteria)
{}

inline bool SolarEclipse::IsEvent(std::shared_ptr<csp::Packet> a_currentReading) noexcept
{
    return details::SunAngle(a_currentReading) <= m_criteria;
}

inline bool PartialSolarEclipse::IsEvent(std::shared_ptr<csp::Packet> a_currentReading) noexcept
{
    return m_criteriaFrom < details::SunAngle(a_currentReading) <= m_criteriaUntil;
}

inline bool FullSunExposure::IsEvent(std::shared_ptr<csp::Packet> a_currentReading) noexcept
{
    return details::SunAngle(a_currentReading) > m_criteria;
}

inline event::EventT SolarEclipse::Event() const noexcept 
{
    return event::SOLAR_ECLIPSE;
}

inline event::EventT PartialSolarEclipse::Event() const noexcept 
{
    return event::PARTIAL_ECLIPSE;
}

inline event::EventT FullSunExposure::Event() const noexcept 
{
    return event::FULL_SUN_EXPOSURE;
}

inline track::TrackerT SolarEclipse::Track() const noexcept 
{
    return track::SUN_TRACKER;
}

inline track::TrackerT PartialSolarEclipse::Track() const noexcept 
{
    return track::SUN_TRACKER;
}

inline track::TrackerT FullSunExposure::Track() const noexcept 
{
    return track::SUN_TRACKER;
}



} // sat

#endif //EVENT_SUN_HXX





