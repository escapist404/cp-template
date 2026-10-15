#ifndef MODULAR_ARITHMETIC_HPP
#define MODULAR_ARITHMETIC_HPP

#include <cassert>
#include <type_traits>

#include "types.hpp"

// 【64 位安全模运算与快速幂 (modular_arithmetic.hpp)】
// - 适用于大模数（高达 64 位有符号整数上限）的基础模运算，内部通过 128 位整数防乘法溢出
// - normalize_mod(x, mod)    : 将任意有符号整数规范化到 [0, mod)
// - add_mod / sub_mod(a,b,m) : 模加与模减
// - multiply_mod(a, b, mod)  : 基于 u128 的无溢出模乘 (a * b) % mod
// - power_mod(base, exp, mod): O(log exp) 模快速幂（要求 exp >= 0, mod >= 1）

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
