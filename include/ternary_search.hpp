#ifndef TERNARY_SEARCH_HPP
#define TERNARY_SEARCH_HPP

#include <cassert>
#include <type_traits>

#include "types.hpp"

// 【整数与实数三分查找单峰极值 (ternary_search.hpp)】
// - 适用于严格单峰（先增后减求最大值，或先减后增求最小值）函数：
// - ternary_search_max / ternary_search_min(low, high, f): 在闭区间 [low, high] 整数域上三分求极值点与极值
// - real_ternary_search_max / real_ternary_search_min(low, high, f, iter): 固定迭代次数的实数域三分查找

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
