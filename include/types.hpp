#ifndef TYPES_HPP
#define TYPES_HPP

#include <complex>
#include <iostream>

// ============================================================================
// 【基础类型别名与 128 位整数输出 (types.hpp)】
// ----------------------------------------------------------------------------
// 1. 类型别名签名：
//    - using i64 = long long;
//      64 位有符号整数，值域 [-2^63, 2^63 - 1]（约 +-9.22e18）。
//    - using u64 = unsigned long long;
//      64 位无符号整数，值域 [0, 2^64 - 1]（约 1.84e19），算术遵循模 2^64 自然溢出。
//    - using i128 = __int128_t;
//      GCC/Clang 128 位有符号整数，值域约 +-1.70e38；不支持整数字面量，需用 (i128)x 转型。
//    - using u128 = __uint128_t;
//      GCC/Clang 128 位无符号整数，值域 [0, 2^128 - 1]，常用于 64 位模乘中间防溢出。
//    - using Real = double;
//      计算几何与数值算法统一使用的双精度浮点类型。
//    - using Complex = std::complex<Real>;
//      复数类型，供 FFT 等频域变换使用。
// 2. 函数签名与接口说明：
//    - inline std::ostream& operator<<(std::ostream& out, const i128 value)
//      功能：向标准输出流按十进制打印任意 128 位有符号整数（含 INT128_MIN 边界值）。
//      参数：out 为目标输出流，value 为待输出的 128 位有符号整数。
//      返回：out 的引用，支持链式流式输出；时间复杂度 O(log10(|value|))。
// ============================================================================

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
