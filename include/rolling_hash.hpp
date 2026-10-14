#ifndef ROLLING_HASH_HPP
#define ROLLING_HASH_HPP

#include <cstdint>
#include <string>
#include <vector>

#include "types.hpp"

// Double-modulus polynomial rolling hash for 0-based substring [l, r) queries in O(1).

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
