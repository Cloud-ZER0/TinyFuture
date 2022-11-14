#include "Future.h"
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

int main() {

	auto [p, f] = future::MakeContract<int>();
	std::thread t1([&p]() {
		std::move(p)->setValue(10);
		std::this_thread::sleep_for(2s);
	});
	t1.detach();
	//std::move(p)->setValue(10);
	auto res = future::getResult(std::move(f));
	std::cout << res.GetValue();

	return 0;
}