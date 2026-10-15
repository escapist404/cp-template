#ifndef TYPES_HPP
#define TYPES_HPP

#include <complex>
#include <iostream>

// 【基础类型别名与 128 位整数输出 (types.hpp)】
// - i64 / u64 : 64 位有符号 / 无符号整数
// - i128 / u128: GCC/Clang 128 位整数（无字面量，需通过 (i128)x 转换；常用于大模数防溢出乘法）
// - Real / Complex: 双精度浮点数与复数类型（用于计算几何与 FFT）
// - operator<< : 支持 std::cout 直接输出任意正负 i128 整数；读入 i128 可使用 fastio.hpp

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
