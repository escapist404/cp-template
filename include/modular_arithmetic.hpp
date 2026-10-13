#ifndef MODULAR_ARITHMETIC_HPP
#define MODULAR_ARITHMETIC_HPP

#include <cassert>
#include <type_traits>

#include "types.hpp"

template <typename T>
constexpr T multiply_mod(T a, T b, T mod) {
	static_assert(std::is_integral_v<T> && sizeof(T) <= sizeof(u64));
	assert(mod > 0);
	if constexpr (std::is_signed_v<T>) {
		const T result = (T)((i128)a * b % mod);
		return result < 0 ? result + mod : result;
	} else {
		return (T)((u128)a * b % mod);
	}
}

template <typename T>
constexpr T power_mod(T base, T exponent, T mod) {
	static_assert(std::is_integral_v<T> && sizeof(T) <= sizeof(u64));
	assert(mod > 0);
	base %= mod;
	if constexpr (std::is_signed_v<T>) {
		assert(exponent >= 0);
		if (base < 0) base += mod;
	}
	T result = 1 % mod;
	while (exponent > 0) {
		if (exponent & 1) result = multiply_mod(result, base, mod);
		base = multiply_mod(base, base, mod);
		exponent >>= 1;
	}
	return result;
}


// Arguments must use the same integer type; results are in [0, mod).

#endif
