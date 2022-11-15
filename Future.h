#pragma once

#include <utility>
#include <iostream>
#include <functional>
#include <atomic>
#include <queue>
#include <coroutine>
#include "Promise.h"
#include "Result.h"


namespace future {

	template <typename T>
	class  Future
	{
	public:
		using callback = std::function<void()>;
		template<typename U>
		friend  res::Result<U> getResult(Future<U>&& fuch);
		template <typename U>
		friend void getResultForCallback(Future<U>&& f);
		template<typename U>
		friend std::pair<Promise<U>*, Future<U>> MakeContract();

	public:
		Future(){};
		~Future();

		// COPPYABLE
		Future(const Future<T>& other);
		Future& operator = (const Future<T>& other);
		Future(Future<T>&& otherMove)			   noexcept;
		Future& operator = (Future<T>&& otherMOve) noexcept;

		// MAIN METHODS
		void subscribe(callback c_) {
			queue.push(std::move(c_));
			getResultForCallback(std::move(*this));
		}

		template <typename U>
		U then(std::function<U(T)> c_) {
			res::Result<T> res = getResult(std::move(*this));
			auto value = res.ValueOrThrow();
			U returnValue = c_(std::move(value));
			return std::move(returnValue);
		}

	private:
		// SERVICE FUNCTIONS
		void MakeFutureByPromise(Promise<T>* prom); 
		Promise<T>* takePromise();

	private:
		Promise<T>* possibleResult = nullptr;
		res::Result<T> result_ = res::Result<T>();
		std::atomic_bool waiter{false};
		std::queue<callback> queue{};
	};

	template <typename T>
	Future<T>::Future<T>(const Future<T>& other) {
		this->possibleResult = other.possibleResult;
		this->result_ = other.result_;
	};

	template<typename T>
	Future<T>& Future<T>::operator = (const Future<T>& other) {
		this->possibleResult = other.possibleResult;
	};

	template<typename T>
	Future<T>::~Future<T>() {
		if (possibleResult != nullptr) {
			delete possibleResult;
		}
	}

	template<typename T>
	Promise<T>* Future<T>::takePromise() {
		return possibleResult;
	}

	template <typename T>
	Future<T>::Future(Future<T>&& otherMove) noexcept {
		possibleResult = otherMove.possibleResult;
		otherMove.possibleResult = nullptr;
		result_ = std::move(otherMove.result_);
	}

	/*template<typename T>
	Future<T>& Future<T>::operator = (Future<T>&& otherMove) {
		possibleResult = otherMove.possibleResult;
		otherMove.possibleResult = nullptr;
		result_ = std::move(otherMove.result_);
	}*/

	template <typename T>
	void Future<T>::MakeFutureByPromise(Promise<T>* prom) {
		possibleResult = prom;
	}

	template<typename T>
	std::pair<Promise<T>*, Future<T>> MakeContract() {
		Future<T> future_;
		Promise<T>* promise_ = new Promise<T>();
		future_.MakeFutureByPromise(promise_);
		return std::make_pair(promise_, std::move(future_));
	}

	template<typename T>
	res::Result<T> getResult(Future<T>&& fuch) {
		while (!fuch.possibleResult->isReady())
		{
			std::cout << "WAITING\n";
		}
		return fuch.possibleResult->makeResult();
	}
	template<typename T>
	void getResultForCallback(Future<T>&& f) {
		while (!f.possibleResult->isReady())
		{
			//std::cout << "WAITING\n";
		}
		f.possibleResult->makeResult();
		if (!f.queue.empty()) {
			auto call = std::move(f.queue.back());
			call();
		}
	}
}

//template<typename T>
//class FutureAwaiter {
//public:
//	explicit FutureAwaiter(future::Future<T> f) : future(std::move(f)) {}
//
//	bool await_ready() {
//		return false;
//	}
//	
//	void await_suspend(std::coroutine_handle<> handle) {
//		std::move(future_).subscribe([this, handle](res::Result<T> res) {
//			this->result_ = std::move(res);
//			handle.resume();
//			});
//	}
//	T await_resume() {
//		return result_.ValueOrThrow();
//	}
//
//private:
//	future::Future<T> future_;
//	res::Result<T> result_;
//};
//
//template<typename T>
//auto operator co_await (future::Future<T> f) {
//	return FutureAwaiter(std::move(f));
//}

