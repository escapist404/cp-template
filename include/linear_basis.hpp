#ifndef LINEAR_BASIS_HPP
#define LINEAR_BASIS_HPP

#include <cstdint>

// ============================================================================
// 【异或线性基 XOR Linear Basis (linear_basis.hpp)】
// ----------------------------------------------------------------------------
// 类签名：class LinearBasis
//   基于高斯消元维护 63 位非负整数 [0, 2^63)（最高位 max_bit = 62）的异或线性空间基。
// 成员函数签名详解（单次操作时间复杂度均为 O(log V) = O(63)，空间 O(63)）：
//   - void insert(std::int64_t value)
//     功能：尝试将非负整数 value (value >= 0) 插入线性基；若与现有基线性无关则填入最高非零位槽位。
//   - bool contains(std::int64_t value) const
//     功能：判断非负整数 value 能否由当前线性基中若干个元素异或表出（【注意】value == 0 恒返回 true，对应空子集异或和）。
//   - std::int64_t max_xor(std::int64_t value = 0) const
//     功能：从高位到低位贪心，返回初始值 value 与线性基中某个子集异或能取得的最大结果（默认 value = 0 即子集最大异或和）。
// ============================================================================

class LinearBasis {
	static constexpr int max_bit = 62;
	std::int64_t basis[max_bit + 1]{};

public:
	// void insert(std::int64_t value) — 功能：尝试将非负整数 value (value >= 0) 插入线性基；若与现有基线性无关则填入最高非零位槽位。
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

	// bool contains(std::int64_t value) const — 功能：判断非负整数 value 能否由当前线性基中若干个元素异或表出（【注意】value == 0 恒返回 true，对应空子集异或和）。
	bool contains(std::int64_t value) const {
		for (int bit = max_bit; bit >= 0; --bit)
			if (value >> bit & 1) {
				if (!basis[bit]) return false;
				value ^= basis[bit];
			}
		return true;
	}

	// std::int64_t max_xor(std::int64_t value = 0) const — 功能：从高位到低位贪心，返回初始值 value 与线性基中某个子集异或能取得的最大结果（默认 value = 0 即子集最大异或和）。
	std::int64_t max_xor(std::int64_t value = 0) const {
		for (int bit = max_bit; bit >= 0; --bit)
			if ((value ^ basis[bit]) > value) value ^= basis[bit];
		return value;
	}
};

#endif
