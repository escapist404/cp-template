#ifndef POLLARD_RHO_HPP
#define POLLARD_RHO_HPP

#include <algorithm>
#include <chrono>
#include <numeric>
#include <random>
#include <vector>

#include "miller_rabin.hpp"

// ============================================================================
// 【Pollard's Rho 64 位大整数质因数分解 (pollard_rho.hpp)】
// ----------------------------------------------------------------------------
// 函数签名与接口详解：
//   1. inline std::vector<i64> factorize(i64 n)
//      功能：主接口，将正整数 n 分解为质因数乘积。
//      返回值：按【升序排列】的质因数列表（含重复质因子，重数等于指数；例如 n = 12 返回 {2, 2, 3}；n <= 1 返回空向量 {}）。
//      复杂度：期望时间复杂度 O(n^(1/4) log n)。
//   2. inline u64 pollard_rho_split(u64 n)
//      功能：对合数 n (n > 1 且非素数) 寻找一个非平凡因子 d (1 < d < n；偶数直接返回 2)。期望 O(n^(1/4))。
//   3. inline void pollard_rho_factor(u64 n, std::vector<i64>& factors)
//      功能：递归分解无符号整数 n，将其全部质因子追加到向量 factors 中（未排序）。
// ============================================================================

// inline u64 pollard_rho_split(u64 n) — 功能：对合数 n (n > 1 且非素数) 寻找一个非平凡因子 d (1 < d < n；偶数直接返回 2)。期望 O(n^(1/4))。
inline u64 pollard_rho_split(u64 n) {
	if (n % 2 == 0) return 2;
	static std::mt19937_64 generator(
			std::chrono::steady_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<u64> distribution(1, n - 1);
	while (true) {
		const u64 c = distribution(generator);
		u64 x = distribution(generator), y = x, divisor = 1;
		auto next = [&](u64 value) {
			return (multiply_mod(value, value, n) + c) % n;
		};
		while (divisor == 1) {
			x = next(x);
			y = next(next(y));
			const u64 difference = x > y ? x - y : y - x;
			divisor = std::gcd(difference, n);
		}
		if (divisor != n) return divisor;
	}
}

// inline void pollard_rho_factor(u64 n, std::vector<i64>& factors) — 功能：递归分解无符号整数 n，将其全部质因子追加到向量 factors 中（未排序）。
inline void pollard_rho_factor(u64 n, std::vector<i64>& factors) {
	if (n == 1) return;
	if (is_prime_mr((i64)n)) {
		factors.push_back((i64)(n));
		return;
	}
	const u64 divisor = pollard_rho_split(n);
	pollard_rho_factor(divisor, factors);
	pollard_rho_factor(n / divisor, factors);
}


// inline std::vector<i64> factorize(i64 n) — 功能：主接口，将正整数 n 分解为质因数乘积。
inline std::vector<i64> factorize(i64 n) {
	if (n < 1) return {};
	std::vector<i64> factors;
	pollard_rho_factor((u64)(n), factors);
	std::sort(factors.begin(), factors.end());
	return factors;
}

#endif
