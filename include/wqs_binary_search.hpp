#ifndef WQS_BINARY_SEARCH_HPP
#define WQS_BINARY_SEARCH_HPP

#include <cassert>
#include <utility>
#include "types.hpp"

// 【WQS 二分 / Aliens Trick 凸包带权二分 (wqs_binary_search.hpp)】
// - 用于消除“恰好选 k 个物品”的个数限制，将问题转化为每个物品附加惩罚斜率 penalty 后的无限制最优值问题
// - wqs_binary_search(low, high, target_count, evaluate): O(log(high - low) * T_eval) 返回消除惩罚后的原问题最优值

template <typename T, typename Solve>
T wqs_binary_search(i64 low, i64 high, int required, Solve solve) {
	assert(low <= high && required >= 0);
	assert(solve(low).first >= required && solve(high).first <= required);
	while (low < high) {
		i64 mid = (i64)((i128)low + ((i128)high - low + 1) / 2);
		if (solve(mid).first >= required) low = mid;
		else high = mid - 1;
	}
	auto result = solve(low);
	return result.second - (T)low * required;
}

#endif
