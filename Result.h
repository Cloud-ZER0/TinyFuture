#pragma once

#include <utility>
#include <exception>
#include "Future.h"

namespace res {

	template<typename T>
	class Result {
	public:
		using Exp = std::exception;
		template<typename U>
		friend Result<U> MakeResult(T value);
	public:
		Result() = default;
		~Result() = default;
		Result(const Result<T>& another);
		Result& operator =(const Result<T>& another) {
			value = another.value;
			except = another.except;
			return *this;
		}
		Result(T&& value);
		T ValueOrThrow() const;
		T GetValue();
	private:
		//void initializeExcept();
		void ThrowExcept();
	private:
		T value = T();
		Exp except = Exp();
	};

	template<typename T>
	Result<T>::Result(T&& value) : value(std::move(value)) {}

	template<typename T>
	Result<T>::Result(const Result<T>& another) : value(another.value) {}

	template<typename T>
	void Result<T>::ThrowExcept() {
		throw except;
	}

	template  <typename T>
	T Result<T>::GetValue() {
		return value;
	}

	template <typename T>
	T Result<T>::ValueOrThrow() const {
		std::abort();
	}

	template <typename T>
	Result<T> MakeResult(T value) {
		Result<T> result(std::move(value));
		return result;
	}

}
