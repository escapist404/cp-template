#ifndef OVERALL_BINARY_SEARCH_HPP
#define OVERALL_BINARY_SEARCH_HPP

#include <cassert>
#include <utility>
#include <vector>
#include "binary_search.hpp"

// 【整体二分 Overall Binary Search (overall_binary_search.hpp)】
// - 基于事件流分治的整体二分框架 overall_binary_search(events, low, high, route, rollback, finish)
// - route(event, mid) 将修改与询问按 mid 路由到左半（返回 true）或右半（返回 false）；每次划分后调用 rollback() 清空临时数据结构

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
