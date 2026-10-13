#ifndef TERNARY_SEARCH_HPP
#define TERNARY_SEARCH_HPP

#include <cassert>
#include <type_traits>

#include "types.hpp"

// Real-valued ternary search on a unimodal function.
template <typename Function>
Real real_ternary_search_max(Real low, Real high, Function function, int iterations = 80) {
	static_assert(std::is_floating_point<Real>::value);

	assert(low <= high && iterations >= 0);
	while (iterations-- > 0) {
		const Real third = (high - low) / 3;
		const Real left = low + third;
		const Real right = high - third;

		if (function(left) < function(right)) {
			low = left;
		} else {
			high = right;
		}
	}
	return low / 2 + high / 2;
}

template <typename Function>
Real real_ternary_search_min(Real low, Real high, Function function, int iterations = 80) {
	return real_ternary_search_max(
		low, high,
		[&](Real x) {
			return -function(x);
		},
		iterations);
}

#endif
