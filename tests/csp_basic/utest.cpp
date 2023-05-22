#include "mu_test.h"

#include <thread>
#include <memory>

#include "csp_boot.hpp"
#include "csp_client_socket.hpp"
#include "csp_server_socket.hpp"
#include "csp_packet.hpp"

constexpr uint8_t exitSignal = 42;
constexpr uint16_t loopback = 0;
constexpr uint8_t PORT = 10;

void Server()
{
	using namespace sat::csp;
	ServerSocket serverSocket{10};
	while (1) {
		auto client = serverSocket.Accept();
		if (client.has_value()) {
			auto receivedMessage = client.value()->Read();
			if (receivedMessage.has_value()) {
				const uint8_t* rawMessage = receivedMessage.value().first->Data();
				if (*rawMessage == exitSignal) {
					return;
				}
			}
		}
	}
}

void Client()
{
	using namespace sat::csp;

	ClientSocket clientSocket{loopback, PORT};
	std::unique_ptr<Packet> messageExit = std::make_unique<Packet>();
	messageExit->Append(exitSignal, sizeof(exitSignal));
	clientSocket.Send(std::move(messageExit));
}

BEGIN_TEST(basic_communication)
	using namespace sat::csp;
	BootCSP();
	std::thread server{Server};
	std::thread client{Client};
	client.join();
	server.join();
	ASSERT_PASS();
END_TEST

BEGIN_SUITE(CubeSat Space Protocol)

	TEST(basic_communication)

END_SUITE