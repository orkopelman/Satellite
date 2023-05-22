#ifndef COLLECTORS_HPP
#define COLLECTORS_HPP

#include "collector_intermediate.hpp"
#include "fake_data_source.hpp"

namespace sat {


class CommunicationCollector: public CollectorIntermediate {
public:
    explicit CommunicationCollector(std::shared_ptr<SqliteDatabase> a_hkDatabase); 
    CommunicationCollector(const CommunicationCollector& a_other) = default;
    CommunicationCollector& operator=(const CommunicationCollector& a_other) = default;
    ~CommunicationCollector() = default;

private:
    size_t waitFor() const override;
    std::unique_ptr<HouseKeepingData> Collect() override;
    void CheckForAnomality(std::string& a_collectedData) override;
    void WriteToTable(std::unique_ptr<HouseKeepingData> a_data) override;

private:
    FakeDataSource m_fakeDataSource;
};


class AdcsCollector: public CollectorIntermediate {
public:
    explicit AdcsCollector(std::shared_ptr<SqliteDatabase> a_hkDatabase); 
    AdcsCollector(const AdcsCollector& a_other) = default;
    AdcsCollector& operator=(const AdcsCollector& a_other) = default;
    ~AdcsCollector() = default;

private:
    size_t waitFor() const override;
    std::unique_ptr<HouseKeepingData> Collect() override;
    void CheckForAnomality(std::string& a_collectedData) override;
    void WriteToTable(std::unique_ptr<HouseKeepingData> a_data) override;

private:
    FakeDataSource m_fakeDataSource;
};


class EpsCollector: public CollectorIntermediate {
public:
    explicit EpsCollector(std::shared_ptr<SqliteDatabase> a_hkDatabase); 
    EpsCollector(const EpsCollector& a_other) = default;
    EpsCollector& operator=(const EpsCollector& a_other) = default;
    ~EpsCollector() = default;

private:
    size_t waitFor() const override;
    std::unique_ptr<HouseKeepingData> Collect() override;
    void CheckForAnomality(std::string& a_collectedData) override;
    void WriteToTable(std::unique_ptr<HouseKeepingData> a_data) override;

private:
    FakeDataSource m_fakeDataSource;
};


class TempCollector: public CollectorIntermediate {
public:
    explicit TempCollector(std::shared_ptr<SqliteDatabase> a_hkDatabase); 
    TempCollector(const TempCollector& a_other) = default;
    TempCollector& operator=(const TempCollector& a_other) = default;
    ~TempCollector() = default;

private:
    size_t waitFor() const override;
    std::unique_ptr<HouseKeepingData> Collect() override;
    void CheckForAnomality(std::string& a_collectedData) override;
    void WriteToTable(std::unique_ptr<HouseKeepingData> a_data) override;

private:
    FakeDataSource m_fakeDataSource;
};


class SunCollector: public CollectorIntermediate {
public:
    explicit SunCollector(std::shared_ptr<SqliteDatabase> a_hkDatabase); 
    SunCollector(const SunCollector& a_other) = default;
    SunCollector& operator=(const SunCollector& a_other) = default;
    ~SunCollector() = default;

private:
    size_t waitFor() const override;
    std::unique_ptr<HouseKeepingData> Collect() override;
    void CheckForAnomality(std::string& a_collectedData) override;
    void WriteToTable(std::unique_ptr<HouseKeepingData> a_data) override;

private:
    FakeDataSource m_fakeDataSource;
};


} //sat

#endif