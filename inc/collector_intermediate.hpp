#ifndef COLLECTOR_INTERMEDIATE_HPP
#define COLLECTOR_INTERMEDIATE_HPP

#include <memory>
#include <vector>

#include "collector_interface.hpp"
#include "housekeeping_data.hpp"
#include "sqlite_database.hpp"
#include "thread_flow.hpp"
#include "emergency_exception.hpp"

namespace sat {

class CollectorIntermediate: public CollectorInterface {
public:
    CollectorIntermediate(std::shared_ptr<SqliteDatabase> a_hkDatabase);
    CollectorIntermediate(const CollectorIntermediate& a_other) = default;
    CollectorIntermediate& operator=(const CollectorIntermediate& a_other) = default;
    ~CollectorIntermediate() = default;

    void Operate() noexcept override;
    void Start() noexcept override;
    void Stop() noexcept override;

protected:
    virtual size_t waitFor() const = 0;
    virtual std::unique_ptr<HouseKeepingData> Collect() = 0;
    virtual void CheckForAnomality(std::string& a_collectedData) = 0; 
    virtual void WriteToTable(std::unique_ptr<HouseKeepingData> a_data) = 0;

protected:
    std::shared_ptr<SqliteDatabase> m_hkDatabase;
    //const Configuration m_config;

private:
    ThreadFlow m_flowController;
};


} //sat

#endif