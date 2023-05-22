#ifndef COLLECTOR_INTERFACE_HPP
#define COLLECTOR_INTERFACE_HPP

namespace sat {

class CollectorInterface {
public:
    virtual ~CollectorInterface() = default;
    virtual void Operate() noexcept = 0;
    virtual void Start() noexcept = 0;
    virtual void Stop() noexcept = 0;

protected:
    CollectorInterface() = default;
    CollectorInterface(const CollectorInterface& a_other) = default;
    CollectorInterface& operator=(const CollectorInterface& a_other) = default;
};

} //sat

#endif