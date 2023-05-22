#ifndef DOWNLINKER_INTERMEDIATE_HPP
#define DOWNLINKER_INTERMEDIATE_HPP

#include <vector>

#include "downlinker_interface.hpp"
#include "thread_flow.hpp"
#include "sqlite_database.hpp"
#include "housekeeping_data.hpp"
#include "parser.hpp"
#include "fsl_exception.hpp"

namespace sat {

class DownlinkerIntermediate: public DownlinkerInterface {
public:
    DownlinkerIntermediate(std::shared_ptr<SqliteDatabase> a_hkDatabase);
    DownlinkerIntermediate(const DownlinkerIntermediate& a_other) = default;
    DownlinkerIntermediate& operator=(const DownlinkerIntermediate& a_other) = default;
    ~DownlinkerIntermediate() = default;

    void Operate() noexcept override;
    void Start() noexcept override;
    void Stop() noexcept override;

protected:
    using DataBlock = std::vector<std::shared_ptr<HouseKeepingData> >;
    virtual subsys::SubsystemsT SubSystem() const noexcept = 0;
    virtual size_t BytesInData() const noexcept = 0;
    virtual std::string ReadFromDatabase() noexcept = 0;
    virtual void InsertIntoBlock(DataBlock& a_dataBlock, std::string& a_data) const = 0;    

protected:
    std::shared_ptr<SqliteDatabase> m_hkDatabase;
    Parser m_parser;

private:
    ThreadFlow m_flowController;
};


class DownlinkerException : public fsl::standard_exception {
    using fsl::standard_exception::standard_exception;
};

} //sat

#endif