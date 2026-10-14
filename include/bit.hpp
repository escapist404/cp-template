#ifndef BIT_HPP
#define BIT_HPP

#include <cassert>
#include <cmath>
#include <limits>
#include <type_traits>

// Integer bit manipulation, power-of-two rounding, log2, and exact integer sqrt utilities.

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
