#ifndef MIN25_HPP
#define MIN25_HPP

#include <cassert>
#include <cmath>
#include <limits>
#include <vector>

#include "bit.hpp"
#include "sieve.hpp"
#include "types.hpp"

// 【Min_25 筛亚线性积性函数前缀和 (min25.hpp)】
// - 在 O(n^(3/4) / log n) 时间与 O(sqrt(n)) 空间内计算 10^10 ~ 10^11 规模的素数个数、素数和及一般积性函数前缀和
// - Min25(n)               : 构造并预处理 <= n 的所有整除分块商点上的素数个数与素数和
// - prime_count() / prime_sum(): O(1) 返回 <= n 的素数个数 pi(n) 与素数总和
// - quotients()            : 按升序返回所有不同的 floor(n / x) 值
// - sum(a, b, prime_power) : 计算满足 f(1)=1、素数处 f(p) = a*p + b、素数幂处为 prime_power(p, e, p^e) 的积性函数前缀和

class Min25 {
public:
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

	// x must be one of floor(n / k), as in all recursive calls.
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
