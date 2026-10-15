#ifndef MODULAR_ARITHMETIC_HPP
#define MODULAR_ARITHMETIC_HPP

#include <cassert>
#include <type_traits>

#include "types.hpp"

// ============================================================================
// 【64 位防溢出安全模乘与模快速幂 (modular_arithmetic.hpp)】
// ----------------------------------------------------------------------------
// 函数签名与接口详解（要求 T 为位宽 <= 64 位的有符号或无符号整型）：
//   1. template <typename T> constexpr T multiply_mod(T a, T b, T mod)
//      功能：计算 (a * b) mod m，返回值严格规范化在 [0, mod) 范围内。
//      实现：有符号类型先归一化至非负，再转型为 128 位整数 (__int128_t / __uint128_t) 相乘后取模，彻底杜绝 64 位乘法溢出。
//      前置条件：mod > 0；时间复杂度 O(1)。
//   2. template <typename T> constexpr T power_mod(T base, T exponent, T mod)
//      功能：二进制快速幂计算 (base ^ exponent) mod m，返回值严格在 [0, mod) 范围内（当 mod == 1 时恒返回 0）。
//      前置条件：mod > 0，且当 T 为有符号类型时要求 exponent >= 0。
//      复杂度：时间复杂度 O(log(exponent))，空间复杂度 O(1)。
// ============================================================================

// template <typename T> constexpr T multiply_mod(T a, T b, T mod) — 功能：计算 (a * b) mod m，返回值严格规范化在 [0, mod) 范围内。
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

// template <typename T> constexpr T power_mod(T base, T exponent, T mod) — 功能：二进制快速幂计算 (base ^ exponent) mod m，返回值严格在 [0, mod) 范围内（当 mod == 1 时恒返回 0）。
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


// 模算术工具：所有参数需使用相同整数类型，返回值保证落在 [0, mod) 内。

#endif
