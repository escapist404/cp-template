#ifndef EULER_HPP
#define EULER_HPP

#include <cassert>
#include <numeric>
#include <string>

#include "bigint.hpp"
#include "modular_arithmetic.hpp"

// ============================================================================
// 【欧拉函数、欧拉定理降幂与扩展欧拉定理（欧拉降幂）(euler.hpp)】
// ----------------------------------------------------------------------------
// 函数签名与接口详解：
//   1. inline i64 euler_phi(i64 n)
//      功能：通过 O(sqrt(n)) 试除法质因数分解计算正整数 n (n > 0) 的欧拉函数值 phi(n)。
//   2. inline i64 euler_power(i64 a, i64 exponent, i64 mod)
//      功能：互质欧拉定理快速幂，计算 a^exponent mod m。
//      【前置条件】要求 mod > 0, exponent >= 0 且 gcd(a, mod) == 1（不互质时触发 assert）。时间复杂度 O(sqrt(mod))。
//   3. inline i64 decimal_mod(const std::string& number, i64 mod)
//      功能：将十进制非负大整数字符串 number 对正整数 mod 取模，返回 [0, mod) 内的余数。时间复杂度 O(|number|)。
//   4. inline bool decimal_at_least(const std::string& number, i64 value)
//      功能：判断无前导零的非负十进制数字串 number 是否 >= 非负整数 value。时间复杂度 O(|number|)。
//   5. inline i64 extended_euler_power(i64 a, const BigInt& exponent, i64 mod)
//   6. inline i64 extended_euler_power(i64 a, const std::string& exponent, i64 mod)
//      功能：扩展欧拉定理（欧拉降幂），在【无需 a 与 mod 互质】的一般情形下计算超大正指数幂 a^exponent mod m：
//            当 exponent < phi(mod) 时直接取 exponent；当 exponent >= phi(mod) 时降幂为 (exponent mod phi(mod)) + phi(mod)。
//      【前置条件】要求 mod > 0 且指数 exponent > 0（非空正整数）。时间复杂度 O(sqrt(mod) + |exponent|)。
// ============================================================================

// inline i64 euler_phi(i64 n) — 功能：通过 O(sqrt(n)) 试除法质因数分解计算正整数 n (n > 0) 的欧拉函数值 phi(n)。
inline i64 euler_phi(i64 n) {
	assert(n > 0);
	i64 result = n;
	for (i64 p = 2; p <= n / p; ++p) {
		if (n % p != 0) continue;
		result = result / p * (p - 1);
		while (n % p == 0) n /= p;
	}
	if (n > 1) result = result / n * (n - 1);
	return result;
}

// inline i64 euler_power(i64 a, i64 exponent, i64 mod) — 功能：互质欧拉定理快速幂，计算 a^exponent mod m。
inline i64 euler_power(i64 a, i64 exponent, i64 mod) {
	assert(mod > 0 && exponent >= 0 && std::gcd(a, mod) == 1);
	return power_mod(a, exponent % euler_phi(mod), mod);
}

// inline i64 decimal_mod(const std::string& number, i64 mod) — 功能：将十进制非负大整数字符串 number 对正整数 mod 取模，返回 [0, mod) 内的余数。时间复杂度 O(|number|)。
inline i64 decimal_mod(const std::string& number, i64 mod) {
	i64 result = 0;
	for (const char digit : number) {
		result = (i64)(((i128)(result) * 10 + digit - '0') % mod);
	}
	return result;
}

// inline bool decimal_at_least(const std::string& number, i64 value) — 功能：判断无前导零的非负十进制数字串 number 是否 >= 非负整数 value。时间复杂度 O(|number|)。
inline bool decimal_at_least(const std::string& number, i64 value) {
	const std::string bound = std::to_string(value);
	if (number.size() != bound.size()) return number.size() > bound.size();
	return number >= bound;
}

// inline i64 extended_euler_power(i64 a, const std::string& exponent, i64 mod) — 功能：扩展欧拉定理（欧拉降幂），在【无需 a 与 mod 互质】的一般情形下计算超大正指数幂 a^exponent mod m：
inline i64 extended_euler_power(i64 a, const BigInt& exponent,
		i64 mod) {
	assert(mod > 0 && !exponent.zero());
	if (mod == 1) return 0;
	const BigInt phi = euler_phi(mod);
	BigInt reduced = exponent % phi;
	if (exponent >= phi) reduced += phi;
	return power_mod(a, reduced.to_i64(), mod);
}

inline i64 extended_euler_power(i64 a, const std::string& exponent,
		i64 mod) {
	assert(mod > 0 && !exponent.empty());
	return extended_euler_power(a, BigInt(exponent), mod);
}

#endif
