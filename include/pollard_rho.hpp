#ifndef POLLARD_RHO_HPP
#define POLLARD_RHO_HPP

#include <algorithm>
#include <chrono>
#include <numeric>
#include <random>
#include <vector>

#include "miller_rabin.hpp"

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


inline std::vector<i64> factorize(i64 n) {
	if (n < 1) return {};
	std::vector<i64> factors;
	pollard_rho_factor((u64)(n), factors);
	std::sort(factors.begin(), factors.end());
	return factors;
}

#endif
