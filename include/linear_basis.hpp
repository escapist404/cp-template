#ifndef LINEAR_BASIS_HPP
#define LINEAR_BASIS_HPP

#include <cstdint>

// XOR linear basis over 63-bit non-negative integers [0, 2^63). O(log V) insert/contains/max_xor.

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
