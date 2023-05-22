#ifndef EVENT_SUN_HPP
#define EVENT_SUN_HPP

#include "intermediate_trackable_event.hpp"
#include "housekeeping_data.hpp"
#include "csp_packet.hpp"

namespace sat {

namespace details {

double AngleCalc(double _a, double _b, double _c);
double SunAngle(std::shared_ptr<csp::Packet> a_currentReading);

} // details

class SolarEclipse : public IntermediateTrackableEvent{
public:
    SolarEclipse(double a_criteria);
    
    bool IsEvent(std::shared_ptr<csp::Packet> a_currentReading) noexcept override;

    event::EventT Event() const noexcept override;
    track::TrackerT Track() const noexcept override;

    double m_criteria;
};

class PartialSolarEclipse : public IntermediateTrackableEvent{
public:
    PartialSolarEclipse(double a_criteriaFrom, double a_criteriaUntil);
    
    bool IsEvent(std::shared_ptr<csp::Packet> a_currentReading) noexcept override;

    event::EventT Event() const noexcept override;
    track::TrackerT Track() const noexcept override;

    double m_criteriaFrom;
    double m_criteriaUntil;
};

class FullSunExposure : public IntermediateTrackableEvent{
public:
    FullSunExposure(double a_criteria);
    
    bool IsEvent(std::shared_ptr<csp::Packet> a_currentReading) noexcept override;

    event::EventT Event() const noexcept override;
    track::TrackerT Track() const noexcept override;

    double m_criteria;
};

} // sat

#include "inl/event_sun.hxx"

#endif //EVENT_SUN_HPP





