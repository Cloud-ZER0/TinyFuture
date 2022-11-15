#pragma once

#include "Result.h"
#include <cassert>

template<typename T>
class Promise {
public:
	Promise() = default;
	~Promise() = default;
	Promise(const Promise<T>& other);
	Promise(std::atomic_bool* waitingPointer) {
		waiterPointer = waitingPointer;
	}
	void setValue(const T& value_);
	res::Result<T> makeResult();
	bool isReady();
private:
	T value = T();
	bool wasAssigned { false };
	std::atomic_bool* waiterPointer = nullptr;
};

template<typename T>
Promise<T>::Promise(const Promise<T>& other): value(other.value) {}

template<typename T>
void Promise<T>::setValue(const T& value_) {
	wasAssigned = true;
	waiterPointer->notify_all();
	value = value_;
}

template <typename T>
res::Result<T> Promise<T>::makeResult() {
	assert(wasAssigned);
	return res::MakeResult(std::move(this->value));
}

template<typename T>
bool Promise<T>::isReady() {
	return wasAssigned;
}