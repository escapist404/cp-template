#ifndef BINARY_SEARCH_HPP
#define BINARY_SEARCH_HPP

// ============================================================================
// 【整数防溢出单调二分与固定迭代实数二分查找 (binary_search.hpp)】
// ----------------------------------------------------------------------------
// 函数签名与接口详解：
//   1. template <class T> constexpr T binary_midpoint(T low, T high)
//      无有符号溢出地计算向下取整中点 floor((low + high) / 2)。O(1)。
//   2. template <class T, class Predicate> T first_true(T low, T high, Predicate predicate)
//      在左闭右开整数区间 [low, high) 上二分（要求先 false 后 true），返回首个 true 位置；全 false 返回 high。
//   3. template <class T, class Predicate> T last_true(T low, T high, Predicate predicate)
//      在左闭右开整数区间 [low, high) 上二分（要求先 true 后 false），返回最后 true 位置；全 false 返回 low - 1。
//   4. template <class T, class Predicate> T real_binary_search(T low, T high, Predicate predicate, int iterations = 80)
//      在实数区间 [low, high] 上固定迭代 iterations 次二分查找先 false 后 true 的分界点。
// ============================================================================

template <class T>
constexpr T binary_midpoint(T low, T high) {
	return low < 0 && high >= 0 ? (low + high) / 2 : low + (high - low) / 2;
}

// 在半开区间 [low, high) 中二分查找第一个满足 predicate(x) == true 的整数 x（单调性：false -> true）；若不存在则返回 high。
template <class T, class Predicate>
T first_true(T low, T high, Predicate predicate) {
	while (low < high) {
		const T mid = binary_midpoint(low, high);
		if (predicate(mid)) {
			high = mid;
		} else {
			low = mid + 1;
		}
	}
	return low;
}

// 在半开区间 [low, high) 中二分查找最后一个满足 predicate(x) == true 的整数 x（单调性：true -> false）；若不存在则返回 low - 1。
template <class T, class Predicate>
T last_true(T low, T high, Predicate predicate) {
	return first_true(low, high, [&](T x) { return !predicate(x); }) - 1;
}

// 实数二分：固定迭代 iterations 次，寻找满足 predicate(x) 由 false 变为 true 的实数分界点。
template <class T, class Predicate>
T real_binary_search(T low, T high, Predicate predicate, int iterations = 80) {
	while (iterations-- > 0) {
		const T mid = low / 2 + high / 2;
		if (predicate(mid)) {
			high = mid;
		} else {
			low = mid;
		}
	}
	return high;
}

#endif
