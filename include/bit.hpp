#ifndef BIT_HPP
#define BIT_HPP

#include <cassert>
#include <cmath>
#include <limits>
#include <type_traits>

// ============================================================================
// 【整数位运算与 64 位精确整数平方根工具 (bit.hpp)】
// ----------------------------------------------------------------------------
// 类型别名签名：
//   - template <typename T> using unsigned_integer_t = std::make_unsigned_t<T>;
//     将任意非 bool 整型 T 转换为等位宽无符号整型。
// 函数签名与接口详解（T 必须为除 bool 外的整型）：
//   - template <typename T> constexpr T lowbit(T value)
//     返回 value 二进制最低位 1 对应的权值（无符号意义下的 x & -x）；value == 0 时返回 0。O(1)。
//   - template <typename T> constexpr T highbit(T value)
//     要求 value >= 0；返回 <= value 的最大 2 次幂（最高位 1 对应的权值）；value == 0 时返回 0。O(log V)。
//   - template <typename T> constexpr T floor_pow2(T value)
//     要求 value >= 0；同 highbit(value)，返回不超过 value 的最大 2 次幂（value == 0 返回 0）。
//   - template <typename T> constexpr T ceil_pow2(T value)
//     要求 value >= 0；返回 >= value 的最小 2 次幂（value <= 1 时返回 1；注意须防最高位左移溢出）。
//   - template <typename T> constexpr int floor_log2(T value)
//     要求 value > 0（传入 0 触发 assert）；返回 floor(log2(value))。
//   - template <typename T> constexpr int bit_width(T value)
//     要求 value >= 0；返回表示 value 所需的二进制有效位数（value == 0 返回 0，否则返回 floor_log2(value) + 1）。
//   - template <typename T> constexpr int ceil_log2(T value)
//     要求 value >= 0；返回满足 2^k >= value 的最小非负整数 k（value <= 1 时返回 0）。
//   - template <typename T> constexpr bool is_pow2(T value)
//     判断 value 在无符号意义下是否为严格正的 2 的整数次幂（即 x > 0 且 (x & (x - 1)) == 0）。O(1)。
//   - template <typename T> T floor_sqrt(T value)
//     要求 value >= 0；返回 floor(sqrt(value))。先用 long double 初估再用整数除法修正，对 64 位整数完全精确无舍入误差。O(1)。
//   - template <typename T> T ceil_sqrt(T value)
//     要求 value >= 0；返回 ceil(sqrt(value))（完全平方数返回 floor_sqrt，否则返回 floor_sqrt + 1）。O(1)。
//   - template <typename T> constexpr int popcount(T value)
//     要求 sizeof(T) <= 8；返回无符号表示下二进制 1 的个数（基于 __builtin_popcountll）。O(1)。
//   - template <typename T> constexpr T bit_reverse(T value, int width)
//     要求 0 <= width <= digits；将 value 的低 width 位前后镜像翻转，高于 width 的位清零。O(width)。
// ============================================================================

template <typename T>
using unsigned_integer_t = std::make_unsigned_t<T>;


template <typename T>
constexpr T lowbit(T value) {
	static_assert(std::is_integral_v<T> && !std::is_same_v<T, bool>);
	using U = std::make_unsigned_t<T>;
	const U x = (U)value;
	return (T)(x & (U)(-x));
}

template <typename T>
constexpr T highbit(T value) {
	static_assert(std::is_integral_v<T> && !std::is_same_v<T, bool>);
	assert(value >= 0);
	using U = std::make_unsigned_t<T>;
	U x = (U)value;
	if (!x) return 0;
	U result = 1;
	while (x >>= 1) result <<= 1;
	return (T)result;
}

template <typename T>
constexpr T floor_pow2(T value) {
	return highbit(value);
}

template <typename T>
constexpr T ceil_pow2(T value) {
	static_assert(std::is_integral_v<T> && !std::is_same_v<T, bool>);
	assert(value >= 0);
	T result = 1;
	while (result < value) result <<= 1;
	return result;
}

template <typename T>
constexpr int floor_log2(T value) {
	static_assert(std::is_integral_v<T> && !std::is_same_v<T, bool>);
	assert(value > 0);
	using U = std::make_unsigned_t<T>;
	U x = (U)value;
	int result = 0;
	while (x >>= 1) ++result;
	return result;
}

template <typename T>
constexpr int bit_width(T value) {
	static_assert(std::is_integral_v<T> && !std::is_same_v<T, bool>);
	assert(value >= 0);
	return value ? floor_log2(value) + 1 : 0;
}

template <typename T>
constexpr int ceil_log2(T value) {
	static_assert(std::is_integral_v<T> && !std::is_same_v<T, bool>);
	assert(value >= 0);
	return value <= 1 ? 0 : bit_width(value - 1);
}

template <typename T>
constexpr bool is_pow2(T value) {
	static_assert(std::is_integral_v<T> && !std::is_same_v<T, bool>);
	using U = std::make_unsigned_t<T>;
	const U x = (U)value;
	return x && !(x & (x - 1));
}

template <typename T>
T floor_sqrt(T value) {
	static_assert(std::is_integral_v<T> && !std::is_same_v<T, bool>);
	assert(value >= 0);
	if (!value) return 0;
	T result = (T)std::sqrt((long double)value);
	while (result + 1 <= value / (result + 1)) ++result;
	while (result > value / result) --result;
	return result;
}

template <typename T>
T ceil_sqrt(T value) {
	T result = floor_sqrt(value);
	return result * result == value ? result : result + 1;
}

template <typename T>
constexpr int popcount(T value) {
	static_assert(std::is_integral_v<T> && !std::is_same_v<T, bool> &&
	              sizeof(T) <= sizeof(unsigned long long));
	using U = std::make_unsigned_t<T>;
	return __builtin_popcountll((unsigned long long)(U)value);
}

template <typename T>
constexpr T bit_reverse(T value, int width) {
	static_assert(std::is_integral_v<T> && !std::is_same_v<T, bool>);
	using U = std::make_unsigned_t<T>;
	assert(0 <= width && width <= std::numeric_limits<U>::digits);
	U x = (U)value, result = 0;
	while (width-- > 0) {
		result = (result << 1) | (x & 1);
		x >>= 1;
	}
	return (T)result;
}

#endif
