#ifndef OVERALL_BINARY_SEARCH_HPP
#define OVERALL_BINARY_SEARCH_HPP

#include <cassert>
#include <utility>
#include <vector>
#include "binary_search.hpp"

// ============================================================================
// 【整体二分框架 Overall Binary Search (overall_binary_search.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   template <typename Event, typename Integer, typename Route, typename Rollback, typename Finish>
//   void overall_binary_search(
//       std::vector<Event> events, Integer low, Integer high,
//       Route route, Rollback rollback, Finish finish)
// 回调签名约定（在闭区间答案域 [low, high] 上递归分流事件）：
//   - `bool route(Event& event, Integer mid)`：返回 true 分入左半 [l, mid]，返回 false 分入右半 [mid + 1, r]；
//   - `void rollback()`                      ：清空或回滚本层 route 产生的临时数据结构修改；
//   - `void finish(Event& event, Integer ans)`：当 l == r == ans 时记录事件最终答案。
// ============================================================================

// template <typename Event, typename Integer, typename Route, typename Rollback, typename Finish> void overall_binary_search( — 功能：Route route, Rollback rollback, Finish finish)
template <typename Event, typename Integer, typename Route, typename Rollback, typename Finish>
void overall_binary_search(std::vector<Event> events, Integer low, Integer high,
	Route route, Rollback rollback, Finish finish) {
	assert(low <= high);
	const auto solve = [&](std::vector<Event>& items, Integer l, Integer r, const auto& self) -> void {
		if (items.empty()) {
			return;
		}
		if (l == r) {
			for (auto& event : items) {
				finish(event, l);
			}
			return;
		}
		const Integer mid = binary_midpoint(l, r);
		std::vector<Event> left, right;
		for (auto& event : items) {
			if (route(event, mid)) {
				left.push_back(std::move(event));
			} else {
				right.push_back(std::move(event));
			}
		}
		rollback();
		std::vector<Event>().swap(items);
		self(left, l, mid, self);
		rollback();
		self(right, mid + 1, r, self);
	};
	solve(events, low, high, solve);
}

#endif
