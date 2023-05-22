#ifndef GARBAGE_COLLECTOR_HPP
#define GARBAGE_COLLECTOR_HPP

#include <memory>
#include <utility>

#include "sqlite_database.hpp"
#include "csp_packet.hpp"

namespace sat {

class GarbageCollector {
public:
    GarbageCollector(std::shared_ptr<SqliteDatabase> a_hkDatabase);
    GarbageCollector(const GarbageCollector& a_other) = default;
    GarbageCollector& operator=(const GarbageCollector& a_other) = default;
    ~GarbageCollector() = default;

    void Clean(std::unique_ptr<csp::Packet> a_packet);

private:
    std::pair<std::string, uint64_t> ExtractInfo(std::unique_ptr<csp::Packet> a_packet) const noexcept;

private:
    std::shared_ptr<SqliteDatabase> m_hkDatabase;
};

} //sat

#endif