#ifndef TERNARY_SEARCH_HPP
#define TERNARY_SEARCH_HPP

#include <cassert>
#include <type_traits>

#include "types.hpp"

// ============================================================================
// 【实数域单峰函数三分查找极值点 Ternary Search (ternary_search.hpp)】
// ----------------------------------------------------------------------------
// 函数签名与接口详解：
//   1. template <typename Function>
//      Real real_ternary_search_max(Real low, Real high, Function function, int iterations = 80)
//      在闭区间 [low, high] 上对先增后减的单峰函数固定迭代 iterations 次，返回取得【最大值】的横坐标 x*。
//   2. template <typename Function>
//      Real real_ternary_search_min(Real low, Real high, Function function, int iterations = 80)
//      在闭区间 [low, high] 上对先减后增的单谷函数固定迭代 iterations 次，返回取得【最小值】的横坐标 x*。
// ============================================================================

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
