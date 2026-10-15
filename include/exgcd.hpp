#ifndef EXGCD_HPP
#define EXGCD_HPP

#include <cstdlib>

#include "types.hpp"

// 【扩展欧几里得与一般模逆元 (exgcd.hpp)】
// - exgcd(a, b, x, y) : 求解裴蜀等式 a*x + b*y = gcd(a, b)，返回非负最大公约数 gcd(|a|, |b|)
// - mod_inverse(a, m) : 求解一般模数（不必为素数）下 a 的乘法逆元；要求 gcd(a, m) == 1，返回 [0, m) 内的唯一逆元

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

inline i64 mod_inverse(i64 a, i64 mod) {
	if (mod <= 0) return -1;
	i64 x, y;
	if (extended_gcd(a, mod, x, y) != 1) return -1;
	x %= mod;
	return x < 0 ? x + mod : x;
}

#endif
