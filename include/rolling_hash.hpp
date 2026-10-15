#ifndef ROLLING_HASH_HPP
#define ROLLING_HASH_HPP

#include <cstdint>
#include <string>
#include <vector>

#include "types.hpp"

// ============================================================================
// 【64 位自然溢出字符串滚动哈希 Rolling Hash (rolling_hash.hpp)】
// ----------------------------------------------------------------------------
// 类签名：class RollingHash
//   采用基数 base = 911382323 在模 2^64 (u64) 下维护字符串前缀哈希（字符映射为 (unsigned char)s[i] + 1）。
//   所有子串区间均遵循 0-based【左闭右开 [l, r)】约定。
// 构造函数与成员函数签名详解：
//   - RollingHash(const std::string& s)
//     功能：预处理长度为 n = s.size() 的字符串 s 的前缀哈希表 hash 与幂次表 power。时间与空间复杂度 O(n)。
//   - Hash get(int l, int r) const
//     功能：返回左闭右开子串 s[l .. r) (0 <= l <= r <= n) 的 64 位无符号哈希值（空串 l == r 返回 0）。O(1)。
//   - bool equal(int l1, int r1, int l2, int r2) const
//     功能：判断两个左闭右开子串 s[l1 .. r1) 与 s[l2 .. r2) 的长度与哈希值是否完全相等。时间复杂度 O(1)。
// ============================================================================

class RollingHash {
	using Hash = u64;
	static constexpr Hash base = 911382323;
	std::vector<Hash> hash, power;

public:
	// RollingHash(const std::string& s) — 功能：预处理长度为 n = s.size() 的字符串 s 的前缀哈希表 hash 与幂次表 power。时间与空间复杂度 O(n)。
	RollingHash(const std::string& s) : hash(s.size() + 1), power(s.size() + 1, 1) {
		for (int i = 0; i < (int)s.size(); ++i) {
			hash[i + 1] = hash[i] * base + (unsigned char)s[i] + 1;
			power[i + 1] = power[i] * base;
		}
	}

	// Hash get(int l, int r) const — 功能：返回左闭右开子串 s[l .. r) (0 <= l <= r <= n) 的 64 位无符号哈希值（空串 l == r 返回 0）。O(1)。
	Hash get(int l, int r) const {
		return hash[r] - hash[l] * power[r - l];
	}

	// bool equal(int l1, int r1, int l2, int r2) const — 功能：判断两个左闭右开子串 s[l1 .. r1) 与 s[l2 .. r2) 的长度与哈希值是否完全相等。时间复杂度 O(1)。
	bool equal(int l1, int r1, int l2, int r2) const {
		return r1 - l1 == r2 - l2 && get(l1, r1) == get(l2, r2);
	}
};

#endif
