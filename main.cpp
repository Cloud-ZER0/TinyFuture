#include "Future.h"
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

int main() {

	auto [p, f] = future::MakeContract<int>();
	std::thread t1([&p]() {
		std::this_thread::sleep_for(2s);
		std::move(p)->setValue(1);
	});
	t1.detach();
	std::move(f).subscribe([]() { // BLOCKING!!!!
		std::cout << "I'm here";
	});
	std::cout << '1\n';
	std::function<int(int)> foo = [](int x) {
		return x + 1;
	};
	//std::move(p)->setValue(10);
	/*auto res = future::getResult(std::move(f));
	std::cout << res.GetValue();*/
	//auto val = std::move(f).then(std::move(foo));
	//std::cout << val;

	return 0;
}