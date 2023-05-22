#include "downlinkers.hpp"

namespace sat {

//Communication downlinker
CommunicationDownlinker::CommunicationDownlinker(std::shared_ptr<SqliteDatabase> a_hkDatabase)
: DownlinkerIntermediate(a_hkDatabase)
, m_bookmark()
{}


subsys::SubsystemsT CommunicationDownlinker::SubSystem() const noexcept
{
    return subsys::COM;
}


size_t CommunicationDownlinker::BytesInData() const noexcept
{
    return sizeof(uint64_t) + sizeof(uint16_t) + sizeof(uint8_t) + sizeof(uint64_t);
}


std::string CommunicationDownlinker::ReadFromDatabase() noexcept
{
    return m_hkDatabase->Read(details::comm_table, m_bookmark);
}


void CommunicationDownlinker::InsertIntoBlock(DataBlock& a_dataBlock, std::string& a_data) const
{   
    if (!a_data.empty()) {
        std::shared_ptr<HouseKeepingData> newData = std::make_shared<HouseKeepingData>(subsys::COM, a_data);
        a_dataBlock.push_back(newData);
    }
}



//Adcs downlinker
AdcsDownlinker::AdcsDownlinker(std::shared_ptr<SqliteDatabase> a_hkDatabase)
: DownlinkerIntermediate(a_hkDatabase)
, m_bookmark()
{}


subsys::SubsystemsT AdcsDownlinker::SubSystem() const noexcept
{
    return subsys::ADCS;
}


size_t AdcsDownlinker::BytesInData() const noexcept
{
    return sizeof(uint64_t) + 3 * sizeof(double);
}


std::string AdcsDownlinker::ReadFromDatabase() noexcept
{
    return m_hkDatabase->Read(details::adcs_table, m_bookmark);
}


void AdcsDownlinker::InsertIntoBlock(DataBlock& a_dataBlock, std::string& a_data) const
{
    if (!a_data.empty()) {
        std::shared_ptr<HouseKeepingData> newData = std::make_shared<HouseKeepingData>(subsys::ADCS, a_data);
        a_dataBlock.push_back(newData);
    }
}



//Eps downlinker
EpsDownlinker::EpsDownlinker(std::shared_ptr<SqliteDatabase> a_hkDatabase)
: DownlinkerIntermediate(a_hkDatabase)
, m_bookmark()
{}


subsys::SubsystemsT EpsDownlinker::SubSystem() const noexcept
{
    return subsys::EPS;
}


size_t EpsDownlinker::BytesInData() const noexcept
{
    return sizeof(uint64_t) + 3 * sizeof(double);
}


std::string EpsDownlinker::ReadFromDatabase() noexcept
{
    return m_hkDatabase->Read(details::eps_table, m_bookmark);
}


void EpsDownlinker::InsertIntoBlock(DataBlock& a_dataBlock, std::string& a_data) const
{
    if (!a_data.empty()) {
        std::shared_ptr<HouseKeepingData> newData = std::make_shared<HouseKeepingData>(subsys::EPS, a_data);
        a_dataBlock.push_back(newData);
    }
}



//Temp downlinker
TempDownlinker::TempDownlinker(std::shared_ptr<SqliteDatabase> a_hkDatabase)
: DownlinkerIntermediate(a_hkDatabase)
, m_bookmark()
{}


subsys::SubsystemsT TempDownlinker::SubSystem() const noexcept
{
    return subsys::TEMP_SENSORS;
}


size_t TempDownlinker::BytesInData() const noexcept
{
    return sizeof(uint64_t) + sizeof(double);
}


std::string TempDownlinker::ReadFromDatabase() noexcept
{
    return m_hkDatabase->Read(details::temp_table, m_bookmark);
}


void TempDownlinker::InsertIntoBlock(DataBlock& a_dataBlock, std::string& a_data) const
{
    std::shared_ptr<HouseKeepingData> newData = std::make_shared<HouseKeepingData>(subsys::TEMP_SENSORS, a_data);
    a_dataBlock.push_back(newData);
}



//Sun downlinker
SunDownlinker::SunDownlinker(std::shared_ptr<SqliteDatabase> a_hkDatabase)
: DownlinkerIntermediate(a_hkDatabase)
, m_bookmark()
{}


subsys::SubsystemsT SunDownlinker::SubSystem() const noexcept
{
    return subsys::SUN_SENSORS;
}


size_t SunDownlinker::BytesInData() const noexcept
{
    return sizeof(uint64_t) + 3 * sizeof(double);
}


std::string SunDownlinker::ReadFromDatabase() noexcept
{
    return m_hkDatabase->Read(details::sun_table, m_bookmark);
}


void SunDownlinker::InsertIntoBlock(DataBlock& a_dataBlock, std::string& a_data) const
{
    std::shared_ptr<HouseKeepingData> newData = std::make_shared<HouseKeepingData>(subsys::SUN_SENSORS, a_data);
    a_dataBlock.push_back(newData);
}


} //sat