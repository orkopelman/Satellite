#include "collectors.hpp"
#include "parser.hpp"
#include <iostream>
namespace sat {


// Communication collector
CommunicationCollector::CommunicationCollector(std::shared_ptr<SqliteDatabase> a_hkDatabase)
: CollectorIntermediate(a_hkDatabase)
, m_fakeDataSource("../fake_data/fake_comm_data.txt")
{}


size_t CommunicationCollector::waitFor() const
{
    // read sleep time from m_config
    return 1;
}


std::unique_ptr<HouseKeepingData> CommunicationCollector::Collect()
{
    std::string rawData = m_fakeDataSource.ReadData();

    try {
        CheckForAnomality(rawData);
    }
    catch (const EmergencyException& e) {
        throw;
    } 

    std::vector<std::string> dataVec = Parser::ParseLine(rawData.data(), details::comm_members);
    return std::make_unique<HouseKeepingData>(details::BuildComData(dataVec));
}


void CommunicationCollector::CheckForAnomality(std::string& a_collectedData)
{
    if (false) { //waiting for config
        throw EmergencyException(a_collectedData);
    }
}


void CommunicationCollector::WriteToTable(std::unique_ptr<HouseKeepingData> a_data)
{
    std::unique_ptr<CommData> comSubdata(dynamic_cast<CommData*>(a_data->sub_data.release()));
    
    char query[details::db_query_length];
    sprintf(query, "INSERT INTO %s (time, ipAddress, port, size) VALUES (%ld, %d, %d, %ld)",
    details::comm_table, a_data->curTime, comSubdata->ipAddress, comSubdata->port, comSubdata->packetSize);
    std::string qStr(query);
    m_hkDatabase->Insert(qStr);
}


// ADCS collector
AdcsCollector::AdcsCollector(std::shared_ptr<SqliteDatabase> a_hkDatabase)
: CollectorIntermediate(a_hkDatabase)
, m_fakeDataSource("../fake_data/fake_adcs_data.txt")
{}


size_t AdcsCollector::waitFor() const
{
    // read sleep time from m_config
    return 1;
}


std::unique_ptr<HouseKeepingData> AdcsCollector::Collect()
{
    std::string rawData = m_fakeDataSource.ReadData();

    try {
        CheckForAnomality(rawData);
    }
    catch (const EmergencyException& e) {
        throw;
    } 

    std::vector<std::string> dataVec = Parser::ParseLine(rawData.data(), details::adcs_members);
    return std::make_unique<HouseKeepingData>(details::BuildAdcsData(dataVec));
}


void AdcsCollector::CheckForAnomality(std::string& a_collectedData)
{
    if (false) { //waiting for config
        throw EmergencyException(a_collectedData);
    }
}


void AdcsCollector::WriteToTable(std::unique_ptr<HouseKeepingData> a_data)
{
    std::unique_ptr<AdcsData> adcsSubdata(dynamic_cast<AdcsData*>(a_data->sub_data.release()));

    char query[details::db_query_length];
    sprintf(query, "INSERT INTO %s (time, xrotationAngle, yrotationAngle, zrotationAngle) VALUES (%ld, %f, %f, %f)", 
    details::adcs_table, a_data->curTime, adcsSubdata->xrotationAngle, adcsSubdata->yrotationAngle, adcsSubdata->zrotationAngle);
    std::string qStr(query);
    m_hkDatabase->Insert(qStr);
}


// EPS collector
EpsCollector::EpsCollector(std::shared_ptr<SqliteDatabase> a_hkDatabase)
: CollectorIntermediate(a_hkDatabase)
, m_fakeDataSource("../fake_data/fake_eps_data.txt")
{}


size_t EpsCollector::waitFor() const
{
    // read sleep time from m_config
    return 1;
}


std::unique_ptr<HouseKeepingData> EpsCollector::Collect()
{
    std::string rawData = m_fakeDataSource.ReadData();

    try {
        CheckForAnomality(rawData);
    }
    catch (const EmergencyException& e) {
        throw;
    } 

    std::vector<std::string> dataVec = Parser::ParseLine(rawData.data(), details::eps_members);
    return std::make_unique<HouseKeepingData>(details::BuildEpsData(dataVec));
}


void EpsCollector::CheckForAnomality(std::string& a_collectedData)
{
    if (false) { //waiting for config
        throw EmergencyException(a_collectedData);
    }
}



void EpsCollector::WriteToTable(std::unique_ptr<HouseKeepingData> a_data)
{
    std::unique_ptr<EpsData> epsSubdata(dynamic_cast<EpsData*>(a_data->sub_data.release()));

    char query[details::db_query_length];
    sprintf(query, "INSERT INTO %s (time, battery, voltage, current) VALUES (%ld, %f, %f, %f)", 
    details::eps_table, a_data->curTime, epsSubdata->batteryPercentage, epsSubdata->voltage, epsSubdata->current);
    std::string qStr(query);
    m_hkDatabase->Insert(qStr);
}


// Temperature collector
TempCollector::TempCollector(std::shared_ptr<SqliteDatabase> a_hkDatabase)
: CollectorIntermediate(a_hkDatabase)
, m_fakeDataSource("../fake_data/fake_temp_data.txt")
{}


size_t TempCollector::waitFor() const
{
    // read sleep time from m_config
    return 1;
}


std::unique_ptr<HouseKeepingData> TempCollector::Collect()
{
    std::string rawData = m_fakeDataSource.ReadData();

    try {
        CheckForAnomality(rawData);
    }
    catch (const EmergencyException& e) {
        throw;
    } 

    std::vector<std::string> dataVec = Parser::ParseLine(rawData.data(), details::temp_members);
    return std::make_unique<HouseKeepingData>(details::BuildTempData(dataVec));
}


void TempCollector::CheckForAnomality(std::string& a_collectedData)
{
    if (false) { //waiting for config
        throw EmergencyException(a_collectedData);
    }

}


void TempCollector::WriteToTable(std::unique_ptr<HouseKeepingData> a_data)
{
    std::unique_ptr<TempData> tempSubdata(dynamic_cast<TempData*>(a_data->sub_data.release()));

    char query[details::db_query_length];
    sprintf(query, "INSERT INTO %s (time, temperatue) VALUES (%ld, %f)", 
    details::temp_table, a_data->curTime, tempSubdata->temperatue);
    std::string qStr(query);
    m_hkDatabase->Insert(qStr);
}


// Sun collector
SunCollector::SunCollector(std::shared_ptr<SqliteDatabase> a_hkDatabase)
: CollectorIntermediate(a_hkDatabase)
, m_fakeDataSource("../fake_data/fake_sun_data.txt")
{}


size_t SunCollector::waitFor() const
{
    // read sleep time from m_config
    return 1;
}


std::unique_ptr<HouseKeepingData> SunCollector::Collect()
{
    std::string rawData = m_fakeDataSource.ReadData();

    try {
        CheckForAnomality(rawData);
    }
    catch (const EmergencyException& e) {
        throw;
    } 

    std::vector<std::string> dataVec = Parser::ParseLine(rawData.data(), details::sun_members);
    return std::make_unique<HouseKeepingData>(details::BuildSunData(dataVec));
}


void SunCollector::CheckForAnomality(std::string& a_collectedData)
{
    if (false) { //waiting for config
        throw EmergencyException(a_collectedData);
    }

}


void SunCollector::WriteToTable(std::unique_ptr<HouseKeepingData> a_data)
{
    std::unique_ptr<SunData> sunSubdata(dynamic_cast<SunData*>(a_data->sub_data.release()));

    char query[details::db_query_length];
    sprintf(query, "INSERT INTO %s (time, xAxisAngle, yAxisAngle, zAxisAngle) VALUES (%ld, %f, %f, %f)", 
    details::sun_table, a_data->curTime, sunSubdata->xAxisAngle, sunSubdata->yAxisAngle, sunSubdata->zAxisAngle);
    std::string qStr(query);
    m_hkDatabase->Insert(qStr);
}


} //sat