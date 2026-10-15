#ifndef BIT_HPP
#define BIT_HPP

#include <cassert>
#include <cmath>
#include <limits>
#include <type_traits>

// 【整数位运算与精确平方根工具 (bit.hpp)】
// - lowbit(x)         : 返回最低位 1 对应的值 x & -x（x = 0 返回 0）
// - highbit / floor_pow2(x): 返回 <= x 的最大 2 次幂（要求 x >= 0，x = 0 返回 0）
// - ceil_pow2(x)      : 返回 >= x 的最小 2 次幂（要求 x >= 0，x <= 1 返回 1）
// - floor_log2(x)     : 返回 floor(log2(x))（要求 x > 0，传入 0 会触发 assert）
// - ceil_log2(x)      : 返回满足 2^k >= x 的最小非负整数 k（要求 x >= 0，x <= 1 返回 0）
// - bit_width(x)      : 返回二进制有效位宽（x = 0 返回 0，x > 0 返回 floor_log2(x) + 1）
// - is_pow2(x)        : 判断无符号意义下是否为严格正的 2 次幂
// - floor_sqrt / ceil_sqrt(x): 64 位精确整数平方根下/上取整（无浮点舍入误差，要求 x >= 0）
// - popcount(x)       : 二进制 1 的个数（支持 <= 64 位整型）
// - bit_reverse(x, w) : 将低 w 位前后翻转（高出 w 的位丢弃）

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
