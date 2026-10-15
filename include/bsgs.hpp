#ifndef BSGS_HPP
#define BSGS_HPP

#include <cassert>
#include <cmath>
#include <numeric>
#include <unordered_map>

#include "bit.hpp"
#include "types.hpp"

// 【大步小步离散对数 BSGS / exBSGS (bsgs.hpp)】
// - 求解离散对数 a^x === b (mod m) 的最小非负整数解 x，支持 gcd(a, m) > 1 的非互质情况 (exBSGS)
// - bsgs(a, b, m): 时间与空间复杂度均为 O(sqrt(m))；若存在非负整数解则返回最小解 x >= 0，若无解则返回 -1

inline i64 bsgs(i64 a, i64 b, i64 mod) {
	assert(mod > 0);
	a %= mod;
	b %= mod;
	if (a < 0) a += mod;
	if (b < 0) b += mod;
	if (mod == 1 || b == 1) return 0;
	auto multiply = [](i64 x, i64 y, i64 m) {
		return (i64)((i128)(x) * y % m);
	};
	i64 multiplier = 1, removed = 0;
	while (true) {
		const i64 g = std::gcd(a, mod);
		if (g == 1) break;
		if (b % g != 0) return -1;
		b /= g;
		mod /= g;
		++removed;
		multiplier = multiply(multiplier, a / g, mod);
		if (multiplier == b) return removed;
	}

	i64 block = ceil_sqrt(mod);
	std::unordered_map<i64, i64> baby;
	i64 value = b, stride = 1;
	for (i64 j = 0; j < block; ++j) {
		baby[value] = j;
		value = multiply(value, a, mod);
		stride = multiply(stride, a, mod);
	}
	value = multiplier;
	for (i64 i = 1; i <= block; ++i) {
		value = multiply(value, stride, mod);
		const auto it = baby.find(value);
		if (it != baby.end()) return i * block - it->second + removed;
	}
	return -1;
}

#endif
