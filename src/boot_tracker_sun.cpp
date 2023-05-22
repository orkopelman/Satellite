#include "event_sun.hpp"
#include "tracker_sun.hpp"

namespace sat {

std::unique_ptr<SunTracker> BootSunTracker()
{
    std::unique_ptr<SunTracker> sunTracker = std::make_unique<SunTracker>(IP , PORT, &dbSun);

    // Create Events
    std::shared_ptr<SolarEclipse> solarEclipse = std::make_shared<SolarEclipse>(CONF_SOLAR_ECLIPSE_CRITERIA);
    std::shared_ptr<PartialSolarEclipse> partialSolarEclipse = std::make_shared<PartialSolarEclipse>();
    std::shared_ptr<FullSunExposure> fullSunExposure = std::make_shared<FullSunExposure>();

    // Load/Add Criterias
    solarEclipse->m_criteria = CONF_SOLAR_ECLIPSE_CRITERIA; // need conf
    partialSolarEclipse->m_criteriaFrom = CONF_PARTIAL_SOLAR_ECLIPSE_CRITERIA_FROM;
    partialSolarEclipse->m_criteriaUntil = CONF_PARTIAL_SOLAR_ECLIPSE_CRITERIA_UNTIL;
    fullSunExposure->m_criteria = CONF_FULL_SUN_EXPOSURE_CRITERIA;

    // Add events to tracker
    sunTracker->Track(solarEclipse);
    sunTracker->Track(partialSolarEclipse);
    sunTracker->Track(fullSunExposure);

    return std::move(sunTracker);
} 

} // sat








