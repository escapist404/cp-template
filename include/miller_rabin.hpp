#ifndef MILLER_RABIN_HPP
#define MILLER_RABIN_HPP

#include "modular_arithmetic.hpp"

inline bool is_prime_mr(i64 n) {
	if (n < 2) return false;
	u64 value = (u64)n;
	for (const u64 p : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
						19ULL, 23ULL, 29ULL, 31ULL, 37ULL}) {
		if (value % p == 0) return value == p;
	}

	u64 d = value - 1;
	int shifts = 0;
	while ((d & 1) == 0) {
		d >>= 1;
		++shifts;
	}
	for (const u64 base : {2ULL, 325ULL, 9375ULL, 28178ULL,
						450775ULL, 9780504ULL, 1795265022ULL}) {
		if (base % value == 0) continue;
		u64 x = power_mod(base % value, d, value);
		if (x == 1 || x == value - 1) continue;
		bool witness = true;
		for (int i = 1; i < shifts; ++i) {
			x = multiply_mod(x, x, value);
			if (x == value - 1) {
				witness = false;
				break;
			}
		}
		if (witness) return false;
	}
	return true;
}

#endif
