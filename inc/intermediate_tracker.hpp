#ifndef INTERMEDIATE_TRACKER_HPP
#define INTERMEDIATE_TRACKER_HPP

#include <memory>
#include <vector>
#include <mutex> 
#include <shared_mutex>

#include "interface_trackable_event.hpp"
#include "interface_tracker.hpp"
#include "sqlite_database.hpp"
#include "thread_flow.hpp"
#include "csp_client_socket.hpp"
#include "muhsat_protocol.hpp"

namespace sat {

class IntermediateTracker : public InterfaceTracker {
public:
    IntermediateTracker(uint16_t a_destAddress, uint8_t a_destPort, const SqliteDatabase& a_db);
    ~IntermediateTracker() = default;

    virtual void Track(std::shared_ptr<InterfaceTrackableEvent> a_trackableEvent) override;
    virtual void Operate() noexcept override; 
    virtual void Start() noexcept override;
    virtual void Stop() noexcept override;

protected:
    virtual const char* TableName() const noexcept = 0;
    virtual std::string CollectData() const noexcept = 0;
    virtual size_t IntermissionLength() const noexcept = 0;
    virtual subsys::SubsystemsT SubSys() const noexcept = 0;
    
protected:
    IntermediateTracker(IntermediateTracker& const bf) = default;
    IntermediateTracker& operator=(const IntermediateTracker& a_other) = default;

    const SqliteDatabase& m_db;

private:
    std::vector<std::shared_ptr<InterfaceTrackableEvent> > m_events;
    mutable std::shared_mutex m_mutex;
    ThreadFlow m_threadFlow;    
    csp::ClientSocket m_socket;
};

} // sat

#endif //INTERMEDIATE_TRACKER_HPP

