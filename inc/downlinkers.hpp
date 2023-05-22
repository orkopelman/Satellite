#ifndef DOWNLINKERS_HPP
#define DOWNLINKERS_HPP

#include "downlinker_intermediate.hpp"

namespace sat {

class CommunicationDownlinker: public DownlinkerIntermediate {
public:
    CommunicationDownlinker(std::shared_ptr<SqliteDatabase> a_hkDatabase);
    CommunicationDownlinker(const CommunicationDownlinker& a_other) = default;
    CommunicationDownlinker& operator=(const CommunicationDownlinker& a_other) = default;
    ~CommunicationDownlinker() = default;

private:
    subsys::SubsystemsT SubSystem() const noexcept override;
    size_t BytesInData() const noexcept override;
    std::string ReadFromDatabase() noexcept override;
    void InsertIntoBlock(DataBlock& a_dataBlock, std::string& a_data) const override;

private:
    DatabaseBookmark m_bookmark;
};


class AdcsDownlinker: public DownlinkerIntermediate {
public:
    AdcsDownlinker(std::shared_ptr<SqliteDatabase> a_hkDatabase);
    AdcsDownlinker(const AdcsDownlinker& a_other) = default;
    AdcsDownlinker& operator=(const AdcsDownlinker& a_other) = default;
    ~AdcsDownlinker() = default;

private:
    subsys::SubsystemsT SubSystem() const noexcept override;
    size_t BytesInData() const noexcept override;
    std::string ReadFromDatabase() noexcept override;
    void InsertIntoBlock(DataBlock& a_dataBlock, std::string& a_data) const override;

private:
    DatabaseBookmark m_bookmark;
};


class EpsDownlinker: public DownlinkerIntermediate {
public:
    EpsDownlinker(std::shared_ptr<SqliteDatabase> a_hkDatabase);
    EpsDownlinker(const EpsDownlinker& a_other) = default;
    EpsDownlinker& operator=(const EpsDownlinker& a_other) = default;
    ~EpsDownlinker() = default;

private:
    subsys::SubsystemsT SubSystem() const noexcept override;
    size_t BytesInData() const noexcept override;
    std::string ReadFromDatabase() noexcept override;
    void InsertIntoBlock(DataBlock& a_dataBlock, std::string& a_data) const override;

private:
    DatabaseBookmark m_bookmark;
};


class TempDownlinker: public DownlinkerIntermediate {
public:
    TempDownlinker(std::shared_ptr<SqliteDatabase> a_hkDatabase);
    TempDownlinker(const TempDownlinker& a_other) = default;
    TempDownlinker& operator=(const TempDownlinker& a_other) = default;
    ~TempDownlinker() = default;

private:
    subsys::SubsystemsT SubSystem() const noexcept override;
    size_t BytesInData() const noexcept override;
    std::string ReadFromDatabase() noexcept override;
    void InsertIntoBlock(DataBlock& a_dataBlock, std::string& a_data) const override;

private:
    DatabaseBookmark m_bookmark;
};


class SunDownlinker: public DownlinkerIntermediate {
public:
    SunDownlinker(std::shared_ptr<SqliteDatabase> a_hkDatabase);
    SunDownlinker(const SunDownlinker& a_other) = default;
    SunDownlinker& operator=(const SunDownlinker& a_other) = default;
    ~SunDownlinker() = default;

private:
    subsys::SubsystemsT SubSystem() const noexcept override;
    size_t BytesInData() const noexcept override;
    std::string ReadFromDatabase() noexcept override;
    void InsertIntoBlock(DataBlock& a_dataBlock, std::string& a_data) const override;

private:
    DatabaseBookmark m_bookmark;
};

} //sat


#endif