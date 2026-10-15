#ifndef MIN25_HPP
#define MIN25_HPP

#include <cassert>
#include <cmath>
#include <limits>
#include <vector>

#include "bit.hpp"
#include "sieve.hpp"
#include "types.hpp"

// ============================================================================
// 【Min_25 筛：亚线性积性函数前缀和 (min25.hpp)】
// ----------------------------------------------------------------------------
// 类签名：class Min25
//   在 O(n^(3/4) / log n) 时间与 O(sqrt(n)) 空间内计算 n <= 10^11 规模的素数个数、素数和及一般积性函数前缀和。
// 构造函数与成员函数签名详解：
//   - Min25(i64 n)
//     功能：给定正整数上界 n (n >= 1)，第一阶段递推预处理所有整除分块点 v = floor(n / k) 处的素数个数 pi(v) 与素数和。
//     复杂度：时间复杂度 O(n^(3/4) / log n)，空间复杂度 O(sqrt(n))。
//   - i64 prime_count() const
//     功能：返回 [1, n] 内的素数总个数 pi(n)。时间复杂度 O(1)。
//   - i64 prime_sum() const
//     功能：返回 [1, n] 内所有素数的总和（若超出 i64 范围会触发 assert）。时间复杂度 O(1)。
//   - std::vector<i64> quotients() const
//     功能：按严格升序返回所有不同的整除分块商值 floor(n / k)。时间复杂度 O(sqrt(n))。
//   - template <typename PrimePower>
//     i64 sum(i64 a, i64 b, const PrimePower& prime_power) const
//     功能：计算一般积性函数 f(x) 的前缀和 sum_{i=1}^n f(i)，要求：
//       1. f(1) == 1；
//       2. 在素数 p 处满足一次多项式形式：f(p) = a * p + b；
//       3. 在素数幂 p^e (e >= 1) 处由回调函数 `i64 prime_power(i64 p, int e, i64 power)` 给出 f(p^e)（其中第三个参数 power == p^e）。
//     复杂度：时间复杂度 O(n^(3/4) / log n)。
// ============================================================================

class Min25 {
public:
	// Min25(i64 n) — 功能：给定正整数上界 n (n >= 1)，第一阶段递推预处理所有整除分块点 v = floor(n / k) 处的素数个数 pi(v) 与素数和。
	Min25(i64 n) : n(n) {
		assert(n >= 1);
		root = floor_sqrt(n);
		assert(root < std::numeric_limits<int>::max());
		primes = sieve_prime((int)(root));
		small.resize(root + 1);
		large.resize(root + 1);
		for (i64 left = 1; left <= n;) {
			const i64 value = n / left;
			const auto id = values.size();
			(value <= root ? small[value] : large[n / value]) = id;
			values.push_back(value);
			counts.push_back(value - 1);
			sums.push_back((i128)(value) * ((i128)(value) + 1) / 2 - 1);
			const i64 right = n / value;
			if (right == n) break;
			left = right + 1;
		}
		prefix.push_back(0);
		for (std::size_t i = 0; i < primes.size(); ++i) {
			const i64 p = primes[i];
			prefix.push_back(prefix.back() + p);
			for (std::size_t j = 0; j < values.size() && p * p <= values[j]; ++j) {
				const auto k = index(values[j] / p);
				counts[j] -= counts[k] - (i64)(i);
				sums[j] -= p * (sums[k] - prefix[i]);
			}
		}
	}

	i64 prime_count() const { return counts.front(); }
	i64 prime_sum() const { return narrow(sums.front()); }

	// std::vector<i64> quotients() const — 功能：按严格升序返回所有不同的整除分块商值 floor(n / k)。时间复杂度 O(sqrt(n))。
	std::vector<i64> quotients() const {
		return std::vector<i64>(values.rbegin(), values.rend());
	}

	// 计算 sum_{i=1}^n f(i)，满足 f(1) = 1 且素数处 f(p) = a*p + b
	// prime_power(p, e, p^e) 返回素数幂处的函数值 f(p^e)
	template <typename PrimePower>
	i64 sum(i64 a, i64 b, const PrimePower& prime_power) const {
		return narrow(1 + sum_from(n, 0, a, b, prime_power));
	}

private:
	i64 n, root;
	std::vector<int> primes;
	std::vector<i64> values, counts;
	std::vector<std::size_t> small, large;
	std::vector<i128> sums, prefix;

	// 查询整除分块位置 x = floor(n / k) 在 primes_le_sqrt 阶段完成后的素数前缀和 F_prime(x)。
	std::size_t index(i64 x) const {
		return x <= root ? small[x] : large[n / x];
	}

	static i64 narrow(const i128 value) {
		assert(value >= std::numeric_limits<i64>::min() &&
				value <= std::numeric_limits<i64>::max());
		return (i64)(value);
	}

	template <typename PrimePower>
	i128 sum_from(i64 x, std::size_t first,
			i64 a, i64 b, const PrimePower& prime_power) const {
		if (x < 2 || (first > 0 && x <= primes[first - 1])) return 0;
		const auto id = index(x);
		i128 result = a * (sums[id] - prefix[first]) +
				(i128)(b) * (counts[id] - (i64)(first));
		for (std::size_t i = first; i < primes.size(); ++i) {
			const i64 p = primes[i];
			if (p > x / p) break;
			i64 power = p;
			for (int e = 1; power <= x / p; ++e) {
				result += (i128)(prime_power(p, e, power)) *
						sum_from(x / power, i + 1, a, b, prime_power);
				power *= p;
				result += prime_power(p, e + 1, power);
			}
		}
		return result;
	}
};

#endif
