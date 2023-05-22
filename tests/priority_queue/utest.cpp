#include "mu_test.h"

#include <thread>
#include <memory>
#include <limits>
#include <vector>
#include <functional>
#include <algorithm>

#include "threads_priority_queue.hpp"

constexpr size_t loopFor = 500'000;

template<typename T>
void Produce(T& a_queue)
{
	for (size_t i = 0; i < loopFor; ++i) {
		try {
			a_queue.Enqueue(static_cast<int>(i % INT64_MAX));
		} catch (const threads::priority_queue_shutdown& e) {
			std::cout << "Shutdown!\n";
			return;
		}
	}
}

template<typename T>
void Consume(T& a_queue)
{
	int buffer;
	for (size_t i = 0; i < loopFor; ++i) {
		try {
			a_queue.Dequeue(buffer);
		} catch (const threads::priority_queue_shutdown& e) {
			std::cout << "Shutdown!\n";
			return;
		}
	}
}

BEGIN_TEST(priority)
	using PQueue = threads::PriorityQueue<int, std::greater<int> >;
	PQueue que{300};
	que.Enqueue(300);
	que.Enqueue(300);
	que.Enqueue(300);
	que.Enqueue(30);
	int popped;
	que.Dequeue(popped);
	ASSERT_EQUAL(popped, 30);
END_TEST

BEGIN_TEST(no_race)
	using PQueue = threads::PriorityQueue<int, std::greater<int> >;
	PQueue que{300, std::move(std::greater<int>{})};
	std::vector<std::thread> threads;
	for (size_t i = 0; i < 10; ++i) {
		threads.push_back(std::thread{Produce<PQueue>, std::ref(que)});
		threads.push_back(std::thread{Consume<PQueue>, std::ref(que)});
	}
	std::for_each(threads.begin(), threads.end(), [](std::thread& a_thread) {
		a_thread.join();
	});
	ASSERT_EQUAL(que.IsEmpty(), true);
END_TEST

BEGIN_TEST(shutdown)
	using PQueue = threads::PriorityQueue<int>;
	PQueue que{300};
	std::vector<std::thread> threads;
	for (size_t i = 0; i < 10; ++i) {
		threads.push_back(std::thread{Produce<PQueue>, std::ref(que)});
		threads.push_back(std::thread{Consume<PQueue>, std::ref(que)});
	}
	que.Shutdown();
	std::for_each(threads.begin(), threads.end(), [](std::thread& a_thread) {
		a_thread.join();
	});
	ASSERT_PASS();
END_TEST

BEGIN_SUITE(Meritocracy Is A Scam)

	TEST(priority)
	TEST(no_race)
	TEST(shutdown)

END_SUITE