#include "mu_test.h"

#include <thread>
#include <memory>
#include <optional>
#include <future>

#include "csp_boot.hpp"
#include "csp_packet.hpp"
#include "csp_client_socket.hpp"
#include "csp_server_socket.hpp"
#include "muhsat_protocol.hpp"
#include "housekeeping_data.hpp"

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

void Client(PacketPtr&& a_message)
{
	using namespace sat::csp;

	ClientSocket clientSocket{loopback, PORT};
	clientSocket.Send(std::move(a_message));
}

PacketPtr TransmitOverNet(PacketPtr a_packet) 
{
	std::promise<PacketPtr> prms;
	std::future<PacketPtr> ftr = prms.get_future();
	std::thread server{Server, std::move(prms)};
	std::thread client{Client, std::move(a_packet)};
	client.join();
	server.join();
	return ftr.get();
}

BEGIN_TEST(ping_test_over_net)
	using namespace sat;
	csp::BootCSP();
	PacketPtr sendMessage = std::make_unique<sat::csp::Packet>();
	Protocol::Ping(*sendMessage, subsys::ANT);
	std::promise<PacketPtr> prms;
	std::future<PacketPtr> ftr = prms.get_future();
	std::thread server{Server, std::move(prms)};
	std::thread client{Client, std::move(sendMessage)};
	client.join();
	server.join();
	PacketPtr received = ftr.get();
	ASSERT_EQUAL(Protocol::Type(*received), prot::PING);
	ASSERT_EQUAL(Protocol::SubProtocol(*received).value(), subsys::ANT);
END_TEST

BEGIN_TEST(ping_test)
	using namespace sat;
	csp::Packet message{};
	Protocol::Ping(message, subsys::ANT);

	ASSERT_EQUAL(Protocol::Type(message), prot::PING);
	ASSERT_EQUAL(Protocol::SubProtocol(message).value(), subsys::ANT);
END_TEST

BEGIN_TEST(pong_test)
	using namespace sat;
	csp::Packet message{};
	Protocol::Pong(message, subsys::ADCS);

	ASSERT_EQUAL(Protocol::Type(message), prot::PONG);
	ASSERT_EQUAL(Protocol::SubProtocol(message).value(), subsys::ADCS);
END_TEST

BEGIN_TEST(panic_test)
	using namespace sat;
	csp::Packet message{};
	Protocol::Panic(message);

	ASSERT_EQUAL(Protocol::Type(message), prot::PANIC);
END_TEST

BEGIN_TEST(command_chain_over_net_test)
	using namespace sat;
	auto send = std::make_unique<csp::Packet>();
	Protocol::CommandChain commChain{};
	commChain.emplace_back(command::SWITCH_MODE, mode::CRITICAL);
	commChain.emplace_back(command::EPS_HANDLER, handle::LOW_POWER);
	Protocol::Command(*send, commChain);

	auto received = TransmitOverNet(std::move(send));
	ASSERT_EQUAL(Protocol::Type(*received), prot::COMMAND);
	auto receivedChainPtr = Protocol::UnpackCommand(*received);
	ASSERT_EQUAL((*receivedChainPtr)[0].first, commChain[0].first);
	ASSERT_EQUAL((*receivedChainPtr)[0].second, commChain[0].second);
	ASSERT_EQUAL((*receivedChainPtr)[1].first, commChain[1].first);
	ASSERT_EQUAL((*receivedChainPtr)[1].second, commChain[1].second);
END_TEST

BEGIN_TEST(delayed_command_chain_over_net_test)
	using namespace sat;
	auto send = std::make_unique<csp::Packet>();
	Protocol::CommandChain commChain{};
	commChain.emplace_back(command::SWITCH_MODE, mode::CRITICAL);
	commChain.emplace_back(command::EPS_HANDLER, handle::LOW_POWER);
	uint64_t delay = INT32_MAX;
	Protocol::DelayCommand(*send, commChain, delay);

	auto received = TransmitOverNet(std::move(send));
	ASSERT_EQUAL(Protocol::Type(*received), prot::DELAYED_COMM);
	ASSERT_EQUAL(Protocol::UnpackDelay(*received), delay);
	auto receivedChainPtr = Protocol::UnpackCommand(*received);
	ASSERT_EQUAL((*receivedChainPtr)[0].first, commChain[0].first);
	ASSERT_EQUAL((*receivedChainPtr)[0].second, commChain[0].second);
	ASSERT_EQUAL((*receivedChainPtr)[1].first, commChain[1].first);
	ASSERT_EQUAL((*receivedChainPtr)[1].second, commChain[1].second);
END_TEST

BEGIN_TEST(hk_received_over_net_test)
	using namespace sat;
	auto send = std::make_unique<csp::Packet>();
	uint64_t timeStamp = INT32_MAX;
	subsys::SubsystemsT subSys = subsys::ADCS;
	Protocol::HousekeepingDataReceived(*send, subSys, timeStamp);

	auto received = TransmitOverNet(std::move(send));
	ASSERT_EQUAL(Protocol::Type(*received), prot::HK_RECEIVED);
	ASSERT_EQUAL(Protocol::SubProtocol(*received).value(), subSys);
	ASSERT_EQUAL(Protocol::UnpackHKDReceivedTimeStamp(*received), timeStamp);
END_TEST

BEGIN_TEST(housekeeping_data_comm_test)
	using namespace sat;
	csp::Packet message{};
	auto commDataSubData = std::make_unique<CommData>(42, 42, 420);
	auto commDataPtr = std::make_shared<HouseKeepingData>(std::move(commDataSubData));
	Protocol::HouseKeepingDataBlock block;
	block.push_back(commDataPtr);
	Protocol::PackHouseKeepingData(message, subsys::COM, block);

	auto receivedData = Protocol::UnpackHouseKeepingData(message)->front();
	ASSERT_EQUAL(receivedData->curTime, commDataPtr->curTime);
	CommData* subDataReceived = dynamic_cast<CommData*>(receivedData->sub_data.get());
	CommData* subDataSent = dynamic_cast<CommData*>(commDataPtr->sub_data.get());
	ASSERT_EQUAL(subDataReceived->ipAddress, subDataSent->ipAddress);
	ASSERT_EQUAL(subDataReceived->port, subDataSent->port);
	ASSERT_EQUAL(subDataReceived->packetSize, subDataSent->packetSize);
END_TEST

BEGIN_TEST(mode_switched_over_net_test)
	using namespace sat;
	auto send = std::make_unique<csp::Packet>();
	uint64_t timeStamp = INT32_MAX;
	mode::ModeT switchedTo = mode::CRITICAL;
	Protocol::SwitchedMode(*send, switchedTo, timeStamp);

	auto received = TransmitOverNet(std::move(send));
	ASSERT_EQUAL(Protocol::Type(*received), prot::MODE_SWITCHED);
	ASSERT_EQUAL(Protocol::SubProtocol(*received).value(), switchedTo);
	ASSERT_EQUAL(Protocol::UnpackModeSwitchedTimeStamp(*received), timeStamp);
END_TEST

BEGIN_TEST(anomaly_test)
	using namespace sat;
	csp::Packet message{};
	std::string txt{"anomaly message raised from exception."};
	uint64_t before = time(nullptr);
	Protocol::Anomaly(message, txt);

	std::unique_ptr<Protocol::AnomalyReport> report = Protocol::UnpackAnomaly(message);
	uint64_t after = time(nullptr);
	ASSERT_THAT(report->second >= before && report->second <= after);
	ASSERT_EQUAL(Protocol::Type(message), prot::ANOMALY);
	ASSERT_EQUAL(report->first, txt);
END_TEST

BEGIN_TEST(event_occured_over_net_test)
	using namespace sat;
	auto send = std::make_unique<csp::Packet>();
	track::TrackerT tracker = track::SUN_TRACKER;
	event::EventT occured = event::SOLAR_ECLIPSE;
	Protocol::EventOccured(*send, tracker, occured);

	auto received = TransmitOverNet(std::move(send));
	ASSERT_EQUAL(Protocol::Type(*received), prot::EVENT_OCCURED);
	ASSERT_EQUAL(Protocol::SubProtocol(*received).value(), tracker);
	ASSERT_EQUAL(Protocol::UnpackEvent(*received), occured);
END_TEST

BEGIN_TEST(handler_request_over_net_test)
	using namespace sat;
	auto send = std::make_unique<csp::Packet>();
	subsys::SubsystemsT subSys = subsys::ADCS;
	handle::HandlerRequestT hand = handle::LOW_POWER;
	Protocol::HandlerRequest(*send, subSys, hand);

	auto received = TransmitOverNet(std::move(send));
	ASSERT_EQUAL(Protocol::Type(*received), prot::HANDLE);
	ASSERT_EQUAL(Protocol::SubProtocol(*received).value(), subSys);
	ASSERT_EQUAL(Protocol::UnpackHandlerRequest(*received), hand);
END_TEST

BEGIN_TEST(reboot_over_net_test)
	using namespace sat;
	auto send = std::make_unique<csp::Packet>();
	subsys::SubsystemsT subSys = subsys::ADCS;
	Protocol::Reboot(*send, subSys);

	auto received = TransmitOverNet(std::move(send));
	ASSERT_EQUAL(Protocol::Type(*received), prot::REBOOT);
	ASSERT_EQUAL(Protocol::SubProtocol(*received).value(), subSys);
END_TEST

BEGIN_TEST(turn_on_over_net_test)
	using namespace sat;
	auto send = std::make_unique<csp::Packet>();
	subsys::SubsystemsT subSys = subsys::ADCS;
	Protocol::TurnOn(*send, subSys);

	auto received = TransmitOverNet(std::move(send));
	ASSERT_EQUAL(Protocol::Type(*received), prot::TURN_ON);
	ASSERT_EQUAL(Protocol::SubProtocol(*received).value(), subSys);
END_TEST

BEGIN_TEST(turn_off_over_net_test)
	using namespace sat;
	auto send = std::make_unique<csp::Packet>();
	subsys::SubsystemsT subSys = subsys::ADCS;
	Protocol::TurnOff(*send, subSys);

	auto received = TransmitOverNet(std::move(send));
	ASSERT_EQUAL(Protocol::Type(*received), prot::TURN_OFF);
	ASSERT_EQUAL(Protocol::SubProtocol(*received).value(), subSys);
END_TEST

BEGIN_SUITE(MuhSat1 Space Protocol)

	TEST(ping_test_over_net)
	TEST(ping_test)
	TEST(pong_test)
	TEST(panic_test)
	TEST(command_chain_over_net_test)
	TEST(delayed_command_chain_over_net_test)
	TEST(hk_received_over_net_test)
	TEST(housekeeping_data_comm_test)
	TEST(mode_switched_over_net_test)
	TEST(anomaly_test)
	TEST(event_occured_over_net_test)
	TEST(handler_request_over_net_test)
	TEST(reboot_over_net_test)
	TEST(turn_on_over_net_test)
	TEST(turn_off_over_net_test)

END_SUITE