#pragma once

#include <utility>
#include <exception>
#include "Future.h"

namespace res {

	template<typename T>
	class Result {
	public:
		template<typename U>
		friend Result<U> MakeResult(T value);

	public:
		Result() {};
		~Result() {};

		Result(const Result<T>& another);
		Result& operator =(const Result<T>& another);
		Result(T&& value);

		T ValueOrThrow() const;
		T GetValue() const ;
	private:
		T value = T();
		std::exception excepted = std::exception();
		bool madeExeception{ false };
	};

	template<typename T>
	Result<T>::Result(T&& value) : value(std::move(value)) {}

	template<typename T>
	Result<T>::Result(const Result<T>& another) : value(another.value) {}

	template <typename T>
	Result<T>& Result<T>::operator =(const Result<T>& another) {
		value = another.value;
		excepted = another.excepted;
		return *this;
	}

	template  <typename T>
	T Result<T>::GetValue() const {
		return value;
	}

	template <typename T>
	T Result<T>::ValueOrThrow() const {
		if(madeExeception) {
			throw excepted;
		}
		else {
			return value;
		}
	}

	template <typename T>
	Result<T> MakeResult(T value) {
		Result<T> result(std::move(value));
		return result;
	}

}
