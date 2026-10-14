#ifndef EXGCD_HPP
#define EXGCD_HPP

#include <cstdlib>

#include "types.hpp"

// Extended Euclidean algorithm and modular inverse for general (possibly non-prime) moduli.

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
