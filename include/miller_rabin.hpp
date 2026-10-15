#ifndef MILLER_RABIN_HPP
#define MILLER_RABIN_HPP

#include "modular_arithmetic.hpp"

// ============================================================================
// 【确定性 Miller-Rabin 64 位素性测试 (miller_rabin.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   inline bool is_prime_mr(i64 n)
// 参数与返回值详解：
//   - n：待判定的 64 位有符号整数（n <= 1 时直接返回 false）。
//   - 实现：采用覆盖 2^64 范围的 7 个确定性基底 {2, 325, 9375, 28178, 450775, 9780504, 1795265022}，
//           配合 128 位防溢出模乘 multiply_mod，在 i64 全范围内【100% 确定性无误判】。
//   - 返回值 bool：若 n 为素数返回 true，否则返回 false。
// 复杂度：时间复杂度 O(7 * log_2(n))，空间复杂度 O(1)。
// ============================================================================

// inline bool is_prime_mr(i64 n) — 功能：参数与返回值详解：
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
