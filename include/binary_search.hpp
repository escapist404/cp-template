#ifndef BINARY_SEARCH_HPP
#define BINARY_SEARCH_HPP

// 【整数与实数单调二分查找 (binary_search.hpp)】
// - binary_midpoint(l, r)            : 无溢出计算向下取整中点 l + (r - l) / 2
// - first_true(low, high, pred)      : 在左闭右开 [low, high) 上二分（要求先假后真），返回首个真值位置；全假返回 high
// - last_true(low, high, pred)       : 在左闭右开 [low, high) 上二分（要求先真后假），返回最后真值位置；全假返回 low - 1
// - real_binary_search(low, high, pred, iter): 固定迭代 iter 次（默认 80 次）的实数二分

template <class T>
constexpr T binary_midpoint(T low, T high) {
	return low < 0 && high >= 0 ? (low + high) / 2 : low + (high - low) / 2;
}

// Search the first x in [low, high) with predicate(x) == true.
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

// Search the last x in [low, high) with predicate(x) == true.
template <class T, class Predicate>
T last_true(T low, T high, Predicate predicate) {
	return first_true(low, high, [&](T x) { return !predicate(x); }) - 1;
}

// Real-valued binary search. The predicate is false, then true.
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
