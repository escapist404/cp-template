#ifndef TYPES_HPP
#define TYPES_HPP

#include <complex>
#include <iostream>

// Common fixed-width integer/float/complex aliases and std::ostream operator<< for i128.

using i64 = long long;
using u64 = unsigned long long;
using i128 = __int128_t;
using u128 = __uint128_t;
using Real = double;
using Complex = std::complex<Real>;

inline std::ostream& operator<<(std::ostream& out, const i128 value) {
	if (value == 0) return out << '0';
	u128 magnitude = value < 0
		? (u128)(-(value + 1)) + 1
		: (u128)(value);
	if (value < 0) out.put('-');
	char digits[40];
	int length = 0;
	while (magnitude > 0) {
		digits[length++] = (char)('0' + magnitude % 10);
		magnitude /= 10;
	}
	while (length > 0) out.put(digits[--length]);
	return out;
}

#endif
