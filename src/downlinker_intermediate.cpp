#include "downlinker_intermediate.hpp"
#include "csp_packet.hpp"
#include "muhsat_protocol.hpp"

namespace sat {


DownlinkerIntermediate::DownlinkerIntermediate(std::shared_ptr<SqliteDatabase> a_hkDatabase)
: m_hkDatabase(a_hkDatabase)
, m_parser()
, m_flowController()
{}


void DownlinkerIntermediate::Operate() noexcept
{
    while (m_flowController.CheckFlow()) {
        std::string data;
        DataBlock block;
        size_t bytesInBlock = 0;

        while (bytesInBlock < csp::MTUinBytes) {
            size_t unitSize = BytesInData();

            if (bytesInBlock + unitSize <= csp::MTUinBytes) {
                data = ReadFromDatabase();
                InsertIntoBlock(block, data);
                bytesInBlock += unitSize;
            } else {
                break;
            }
        }
        
        csp::Packet packet;
        Protocol::PackHouseKeepingData(packet, SubSystem(), block);
        // send packet to router
    }
}


void DownlinkerIntermediate::Start() noexcept
{
    m_flowController.Start();
}


void DownlinkerIntermediate::Stop() noexcept
{
    m_flowController.Stop();
}


} //sat