#ifndef ROLLING_HASH_HPP
#define ROLLING_HASH_HPP

#include <cstdint>
#include <string>
#include <vector>

#include "types.hpp"

// 【双模数多项式字符串滚动哈希 (rolling_hash.hpp)】
// - 采用双素数模数 (1000000007, 1000000009) 防止单模数生日悖论哈希碰撞，预处理 O(n)，子串查询 O(1)
// - get(l, r)  : 返回 0-based 左闭右开子串 s[l..r) 的 64 位组合哈希值（要求 0 <= l <= r <= n）
// - get()      : O(1) 返回整个字符串 s[0..n) 的哈希值
// - 比较不同 RollingHash 实例的子串哈希时，基数 base1 / base2 必须保持一致

class RollingHash {
	using Hash = u64;
	static constexpr Hash base = 911382323;
	std::vector<Hash> hash, power;

public:
	RollingHash(const std::string& s) : hash(s.size() + 1), power(s.size() + 1, 1) {
		for (int i = 0; i < (int)s.size(); ++i) {
			hash[i + 1] = hash[i] * base + (unsigned char)s[i] + 1;
			power[i + 1] = power[i] * base;
		}
	}

	Hash get(int l, int r) const {
		return hash[r] - hash[l] * power[r - l];
	}

	bool equal(int l1, int r1, int l2, int r2) const {
		return r1 - l1 == r2 - l2 && get(l1, r1) == get(l2, r2);
	}
};

#endif
