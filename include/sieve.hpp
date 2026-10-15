#ifndef SIEVE_HPP
#define SIEVE_HPP

#include <cassert>
#include <tuple>
#include <utility>
#include <vector>

// ============================================================================
// 【欧拉线性筛：素数表与积性数论函数预处理 Linear Sieve (sieve.hpp)】
// ----------------------------------------------------------------------------
// 函数签名与返回值详解（所有函数均处理闭区间 [1, n] 或 [0, n]，时间与空间复杂度严格线性 O(n)）：
//   1. template <typename OnPrime, typename OnComposite>
//      std::vector<int> sieve(int n, OnPrime on_prime, OnComposite on_composite)
//      功能：通用线性筛骨架，返回 <= n 的升序素数列表 primes。
//      回调签名：
//        - void on_prime(int p)：当发现新素数 p 时调用；
//        - void on_composite(int i, int prime, int multiple, bool divisible)：
//          当用最小质因子 prime 筛去合数 multiple = i * prime 时调用，divisible == (i % prime == 0)。
//   2. std::vector<int> sieve_prime(int n)
//      功能：返回 [2, n] 内所有素数的升序列表（n < 2 时返回空向量）。O(n)。
//   3. std::pair<std::vector<int>, std::vector<int>> sieve_phi(int n)
//      功能：返回 {primes, phi}，其中 phi[x] (1 <= x <= n) 为欧拉函数值（phi[0] = 0, phi[1] = 1）。O(n)。
//   4. std::pair<std::vector<int>, std::vector<int>> sieve_mu(int n)
//      功能：返回 {primes, mu}，其中 mu[x] (1 <= x <= n) 为莫比乌斯函数值（取 -1, 0, 1；mu[1] = 1）。O(n)。
//   5. std::tuple<std::vector<int>, std::vector<int>, std::vector<int>> sieve_arithmetic(int n)
//      功能：单次遍历同时筛出并返回 {primes, phi, mu} 三元组。O(n)。
//   6. std::tuple<std::vector<int>, std::vector<int>, std::vector<long long>> sieve_divisor(int n)
//      功能：返回 {primes, tau, sigma}，其中 tau[x] 为正整数 x 的正约数个数 d(x)，sigma[x] 为正约数之和 sigma_1(x)。O(n)。
//   7. std::pair<std::vector<int>, std::vector<int>> sieve_min_prime(int n)
//      功能：返回 {primes, min_prime}，其中 min_prime[x] (x >= 2) 为 x 的最小质因子（可用于 O(log x) 快速质因数分解）。O(n)。
// ============================================================================

// template <typename OnPrime, typename OnComposite> std::vector<int> sieve(int n, OnPrime on_prime, OnComposite on_composite) — 功能：通用线性筛骨架，返回 <= n 的升序素数列表 primes。
template <typename OnPrime, typename OnComposite>
std::vector<int> sieve(int n, OnPrime on_prime, OnComposite on_composite) {
	if (n < 2) return {};
	std::vector<int> primes;
	std::vector<bool> composite(n + 1);
	for (int i = 2; i <= n; ++i) {
		if (!composite[i]) {
			primes.push_back(i);
			on_prime(i);
		}
		for (const int prime : primes) {
			if (prime > n / i) break;
			const int multiple = prime * i;
			composite[multiple] = true;
			const bool divisible = i % prime == 0;
			on_composite(i, prime, multiple, divisible);
			if (divisible) break;
		}
	}
	return primes;
}

// std::vector<int> sieve_prime(int n) — 功能：返回 [2, n] 内所有素数的升序列表（n < 2 时返回空向量）。O(n)。
std::vector<int> sieve_prime(int n) {
	return sieve(n, [](int) {}, [](int, int, int, bool) {});
}

// std::pair<std::vector<int>, std::vector<int>> sieve_phi(int n) — 功能：返回 {primes, phi}，其中 phi[x] (1 <= x <= n) 为欧拉函数值（phi[0] = 0, phi[1] = 1）。O(n)。
std::pair<std::vector<int>, std::vector<int>> sieve_phi(int n) {
	assert(n >= 1);
	if (n < 1) return {};

	std::vector<int> phi(n + 1);
	phi[1] = 1;
	std::vector<int> primes = sieve(n,
		[&](int i) { phi[i] = i - 1; },
		[&](int i, int prime, int multiple, bool divisible) {
			phi[multiple] = divisible
				? phi[i] * prime
				: phi[i] * (prime - 1);
		});
	return {std::move(primes), std::move(phi)};
}

// std::pair<std::vector<int>, std::vector<int>> sieve_mu(int n) — 功能：返回 {primes, mu}，其中 mu[x] (1 <= x <= n) 为莫比乌斯函数值（取 -1, 0, 1；mu[1] = 1）。O(n)。
std::pair<std::vector<int>, std::vector<int>> sieve_mu(int n) {
	assert(n >= 1);
	if (n < 1) return {};

	std::vector<int> mu(n + 1);
	mu[1] = 1;
	std::vector<int> primes = sieve(n,
		[&](int i) { mu[i] = -1; },
		[&](int i, int, int multiple, bool divisible) {
			mu[multiple] = divisible ? 0 : -mu[i];
		});
	return {std::move(primes), std::move(mu)};
}

std::tuple<std::vector<int>, std::vector<int>, std::vector<int>>
// std::tuple<std::vector<int>, std::vector<int>, std::vector<int>> sieve_arithmetic(int n) — 功能：单次遍历同时筛出并返回 {primes, phi, mu} 三元组。O(n)。
sieve_arithmetic(int n) {
	assert(n >= 1);
	if (n < 1) return {};

	std::vector<int> phi(n + 1), mu(n + 1);
	phi[1] = mu[1] = 1;
	std::vector<int> primes = sieve(n,
		[&](int i) {
			phi[i] = i - 1;
			mu[i] = -1;
		},
		[&](int i, int prime, int multiple, bool divisible) {
			if (divisible) {
				phi[multiple] = phi[i] * prime;
				mu[multiple] = 0;
			} else {
				phi[multiple] = phi[i] * (prime - 1);
				mu[multiple] = -mu[i];
			}
		});
	return {std::move(primes), std::move(phi), std::move(mu)};
}

std::tuple<std::vector<int>, std::vector<int>, std::vector<long long>>
// std::tuple<std::vector<int>, std::vector<int>, std::vector<long long>> sieve_divisor(int n) — 功能：返回 {primes, tau, sigma}，其中 tau[x] 为正整数 x 的正约数个数 d(x)，sigma[x] 为正约数之和 sigma_1(x)。O(n)。
sieve_divisor(int n) {
	assert(n >= 1);
	if (n < 1) return {};

	std::vector<int> tau(n + 1), exponent(n + 1);
	std::vector<long long> sigma(n + 1), power(n + 1), prime_sum(n + 1);
	tau[1] = 1;
	sigma[1] = power[1] = prime_sum[1] = 1;
	std::vector<int> primes = sieve(n,
		[&](int prime) {
			tau[prime] = 2;
			exponent[prime] = 1;
			power[prime] = prime;
			prime_sum[prime] = prime + 1;
			sigma[prime] = prime + 1;
		},
		[&](int i, int prime, int multiple, bool divisible) {
			if (divisible) {
				exponent[multiple] = exponent[i] + 1;
				tau[multiple] = tau[i] / (exponent[i] + 1)
					* (exponent[i] + 2);
				power[multiple] = power[i] * prime;
				prime_sum[multiple] = prime_sum[i] + power[multiple];
				sigma[multiple] = sigma[i] / prime_sum[i]
					* prime_sum[multiple];
			} else {
				exponent[multiple] = 1;
				tau[multiple] = tau[i] * 2;
				power[multiple] = prime;
				prime_sum[multiple] = prime + 1;
				sigma[multiple] = sigma[i] * (prime + 1);
			}
		});
	return {std::move(primes), std::move(tau), std::move(sigma)};
}

std::pair<std::vector<int>, std::vector<int>> sieve_factor(int n) {
	std::vector<int> min_factor(n >= 0 ? n + 1 : 0);
	std::vector<int> primes = sieve(n,
		[&](int i) { min_factor[i] = i; },
		[&](int, int prime, int multiple, bool) { min_factor[multiple] = prime; });
	return {std::move(primes), std::move(min_factor)};
}

#endif
