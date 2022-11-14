#pragma once

#include <utility>
#include <iostream>
#include "Promise.h"
#include "Result.h"
#include <atomic>

namespace future {

	template <typename T>
	class  Future
	{
	public:
		template<typename U>
		friend  res::Result<U> getResult(Future<U>&& fuch);
		template<typename U>
		friend std::pair<Promise<U>*, Future<U>> MakeContract();

		Future() = default;
		~Future() {
			if (possibleResult != nullptr) {
				delete possibleResult;
			}
		}

		// NON COPPYABLE
		Future(const Future<T>& other) {
			this->possibleResult = other.possibleResult;
			this->resl = other.resl;
		};
		Future& operator = (const Future<T>& other) {
			this->possibleResult = other.possibleResult;
		};
		Future(Future<T>&& otherMOVE) noexcept;

	private:
		// SERVICE FUNCTIONS
		void MakeFutureByPromise(Promise<T>* prom); // Promise<T>* prom 
		Promise<T>* takePromise();

	private:
		Promise<T>* possibleResult = nullptr;
		Result<T> resl = Result<T>();
		std::atomic_bool waiter{false};
	};

	template<typename T>
	Promise<T>* Future<T>::takePromise() {
		return possibleResult;
	}

	template <typename T>
	Future<T>::Future(Future<T>&& otherMOVE) noexcept {
		possibleResult = otherMOVE.possibleResult;
		otherMOVE.possibleResult = nullptr;
	}

	template <typename T>
	void Future<T>::MakeFutureByPromise(Promise<T>* prom) {
		possibleResult = prom;
		//return *possibleResult;
	}

	template<typename T>
	std::pair<Promise<T>*, Future<T>> MakeContract() {
		Future<T> future_;
		Promise<T>* promise_ = new Promise<T>();
		future_.MakeFutureByPromise(promise_);
		return std::make_pair(promise_, std::move(future_));
		std::abort();
	}

	template<typename T>
	res::Result<T> getResult(Future<T>&& fuch) {
		while (!fuch.possibleResult->isReady())
		{
			std::cout << "WAITING\n";
		}
		return fuch.possibleResult->makeResult();
	}
}
