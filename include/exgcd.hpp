#ifndef EXGCD_HPP
#define EXGCD_HPP

#include <cstdlib>

#include "types.hpp"

// ============================================================================
// 【扩展欧几里得算法与任意模数乘法逆元 Extended GCD & Modular Inverse (exgcd.hpp)】
// ----------------------------------------------------------------------------
// 函数签名与接口详解：
//   1. inline i64 extended_gcd(i64 a, i64 b, i64& x, i64& y)
//      功能：求解裴蜀等式（Bézout's identity）a * x + b * y = gcd(a, b)。
//      参数与返回值约定：
//        - a, b：任意 64 位有符号整数（可正、可负、可为 0）；
//        - x, y：引用输出参数，写入满足 a * x + b * y == g 的一组特解；
//        - 返回值 i64：始终返回【非负】最大公约数 g = gcd(|a|, |b|)（仅当 a == 0 && b == 0 时返回 0）。
//      复杂度：时间复杂度 O(log(min(|a|, |b|)))，空间复杂度 O(1)。
//   2. inline i64 mod_inverse(i64 a, i64 mod)
//      功能：求整数 a 在模 mod 下的乘法逆元 x，满足 a * x === 1 (mod m)（不要求 mod 为素数，仅需 gcd(a, mod) == 1）。
//      返回值约定：
//        - 若 mod > 0 且 gcd(a, mod) == 1，返回区间 [0, mod) 内的唯一最小非负逆元（特例 mod == 1 时返回 0）；
//        - 若 mod <= 0 或 gcd(a, mod) != 1（逆元不存在），返回【-1】。
//      复杂度：时间复杂度 O(log(mod))。
// ============================================================================

// inline i64 extended_gcd(i64 a, i64 b, i64& x, i64& y) — 功能：求解裴蜀等式（Bézout's identity）a * x + b * y = gcd(a, b)。
inline i64 extended_gcd(i64 a, i64 b,
		i64& x, i64& y) {
	i64 old_r = std::llabs(a), r = std::llabs(b);
	i64 old_x = 1, current_x = 0;
	i64 old_y = 0, current_y = 1;
	while (r != 0) {
		const i64 quotient = old_r / r;
		const i64 next_r = old_r - quotient * r;
		old_r = r;
		r = next_r;
		const i64 next_x = old_x - quotient * current_x;
		old_x = current_x;
		current_x = next_x;
		const i64 next_y = old_y - quotient * current_y;
		old_y = current_y;
		current_y = next_y;
	}
	x = a < 0 ? -old_x : old_x;
	y = b < 0 ? -old_y : old_y;
	return old_r;
}

// inline i64 mod_inverse(i64 a, i64 mod) — 功能：求整数 a 在模 mod 下的乘法逆元 x，满足 a * x === 1 (mod m)（不要求 mod 为素数，仅需 gcd(a, mod) == 1）。
inline i64 mod_inverse(i64 a, i64 mod) {
	if (mod <= 0) return -1;
	i64 x, y;
	if (extended_gcd(a, mod, x, y) != 1) return -1;
	x %= mod;
	return x < 0 ? x + mod : x;
}

#endif
