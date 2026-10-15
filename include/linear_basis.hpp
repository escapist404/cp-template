#ifndef LINEAR_BASIS_HPP
#define LINEAR_BASIS_HPP

#include <cstdint>

// 【异或线性基 (linear_basis.hpp)】
// - 维护 63 位非负整数 [0, 2^63) 的异或线性基（最高位 max_bit = 62），单次操作 O(log V)
// - insert(val)  : 尝试将非负整数 val 插入线性基
// - contains(val): 判断 val 能否由基中元素异或表出（注意 0 恒返回 true）
// - max_xor(val) : 返回初始值 val 与基中子集异或能取得的最大值（默认 val = 0 即子集最大异或和）

class LinearBasis {
	static constexpr int max_bit = 62;
	std::int64_t basis[max_bit + 1]{};

public:
	void insert(std::int64_t value) {
		for (int bit = max_bit; bit >= 0; --bit) {
			if (!(value >> bit & 1)) continue;
			if (basis[bit]) value ^= basis[bit];
			else {
				basis[bit] = value;
				return;
			}
		}
	}

	bool contains(std::int64_t value) const {
		for (int bit = max_bit; bit >= 0; --bit)
			if (value >> bit & 1) {
				if (!basis[bit]) return false;
				value ^= basis[bit];
			}
		return true;
	}

	std::int64_t max_xor(std::int64_t value = 0) const {
		for (int bit = max_bit; bit >= 0; --bit)
			if ((value ^ basis[bit]) > value) value ^= basis[bit];
		return value;
	}
};

#endif
