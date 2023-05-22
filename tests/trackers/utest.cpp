#include "mu_test.h"

#include <thread>
#include <memory>
#include <optional>
#include <future>

#include "sqlite_database.hpp"
#include "csp_boot.hpp"
#include "csp_packet.hpp"
#include "csp_client_socket.hpp"
#include "csp_server_socket.hpp"
#include "muhsat_protocol.hpp"
#include "housekeeping_data.hpp"
#include "tracker_sun.hpp"
#include "event_sun.hpp"
#include "intermediate_tracker.hpp"

constexpr uint16_t loopback = 0;
constexpr uint8_t PORT = 10;

using PacketPtr = std::unique_ptr<sat::csp::Packet>;

void Server(std::promise<PacketPtr>&& prms)
{
	using namespace sat::csp;
	ServerSocket serverSocket{10};
	while (1) {
		auto client = serverSocket.Accept();
		if (client.has_value()) {
			auto receivedMessage = client.value()->Read();
			if (receivedMessage.has_value()) {
				prms.set_value(std::move(receivedMessage.value().first));
				return;
			}
		}
	}
}

std::future<PacketPtr>&& SetServer() 
{
	sat::csp::BootCSP();
	std::promise<PacketPtr> prms;
	std::future<PacketPtr> ftr = prms.get_future();
	std::thread server{Server, std::move(prms)};
	server.detach();
	return std::move(ftr);
}

void BootSunTracker() {
	using namespace sat;
	constexpr double barier = 9.0;
	// intialize fake sun db table access that contains only sun eclipse event triggers

	SqliteDatabase sqldb("../../sql/housekeeping.sqlt");
	SunTracker trackerSun(loopback, PORT, sqldb);
	trackerSun.Track(std::make_shared<SolarEclipse>(barier));
	trackerSun.Operate();
}

BEGIN_TEST(tracking_sun_eclipse)
	using namespace sat;
	auto futureResult = SetServer();
	
	std::thread trackerSun{BootSunTracker};
	trackerSun.detach();

	PacketPtr resultPacket = futureResult.get();
	// check that resultPacket is eventOccured sun eclipse
	ASSERT_EQUAL(Protocol::Type(*resultPacket), prot::EVENT_OCCURED);
	ASSERT_EQUAL(Protocol::SubProtocol(*resultPacket).value(), track::SUN_TRACKER);
	ASSERT_EQUAL(Protocol::UnpackEvent(*resultPacket), event::SOLAR_ECLIPSE);
END_TEST

BEGIN_SUITE(Track Them Events)

	TEST(tracking_sun_eclipse)

END_SUITE