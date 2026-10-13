#ifndef BINARY_SEARCH_HPP
#define BINARY_SEARCH_HPP

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
