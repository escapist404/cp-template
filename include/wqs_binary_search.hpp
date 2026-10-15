#ifndef WQS_BINARY_SEARCH_HPP
#define WQS_BINARY_SEARCH_HPP

#include <cassert>
#include <utility>
#include "types.hpp"

// ============================================================================
// 【WQS 二分 / Aliens Trick 带权惩罚凸包二分 (wqs_binary_search.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   template <typename T, typename Solve>
//   T wqs_binary_search(i64 low, i64 high, int required, Solve solve)
// 参数与契约详解：
//   - 在闭区间 [low, high] 上二分惩罚斜率 mid，回调签名为 `std::pair<int, T> solve(i64 penalty)`，
//     返回 {所选物品个数 count, 含 +penalty * count 在内的增广目标值}。
//   - 要求随 penalty 增大，count 单调不增（即 `solve(low).first >= required >= solve(high).first`）；
//     返回扣除惩罚项后的原问题真实最优值 `solve(opt).second - opt * required`。调用次数 O(log(high - low))。
// ============================================================================

// template <typename T, typename Solve> T wqs_binary_search(i64 low, i64 high, int required, Solve solve) — 功能：T wqs_binary_search(i64 low, i64 high, int required, Solve solve)
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
