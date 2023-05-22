#ifndef DOWNLINKER_INTERFACE_HPP
#define DOWNLINKER_INTERFACE_HPP

namespace sat {

class DownlinkerInterface {
public:
    ~DownlinkerInterface() = default;
    virtual void Operate() noexcept = 0;
    virtual void Start() noexcept = 0;
    virtual void Stop() noexcept = 0;

protected:
    DownlinkerInterface() = default;
    DownlinkerInterface(const DownlinkerInterface& a_other) = default;
    DownlinkerInterface& operator=(const DownlinkerInterface& a_other) = default;
};

} //sat

#endif