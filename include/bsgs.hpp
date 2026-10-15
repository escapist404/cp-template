#ifndef BSGS_HPP
#define BSGS_HPP

#include <cassert>
#include <cmath>
#include <numeric>
#include <unordered_map>

#include "bit.hpp"
#include "types.hpp"

// ============================================================================
// 【大步小步法与扩展大步小步法求离散对数 BSGS / exBSGS (bsgs.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   inline i64 bsgs(i64 a, i64 b, i64 mod)
// 参数与返回值详解：
//   - 功能：求解离散对数同余方程 a^x === b (mod m) 的【最小非负整数解 x >= 0】。
//   - 参数：底数 a、目标余数 b（均可为任意有符号整数，内部自动取模归正），模数 mod（要求 mod > 0）。
//   - 【支持非互质模数】内部先循环提取 g = gcd(a, mod) 化归为互质情形（即完整 exBSGS），无需 gcd(a, mod) == 1。
//   - 【0^0 约定】当 mod == 1 或 b % mod == 1 时，遵循 x = 0 时 a^0 = 1 的数学约定直接返回 0。
//   - 返回值 i64：若存在非负整数解，返回最小解 x >= 0；若方程无解，返回【-1】。
// 复杂度：时间与空间复杂度均为 O(sqrt(mod))。
// ============================================================================

// inline i64 bsgs(i64 a, i64 b, i64 mod) — 功能：参数与返回值详解：
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
