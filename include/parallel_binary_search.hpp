#ifndef PARALLEL_BINARY_SEARCH_HPP
#define PARALLEL_BINARY_SEARCH_HPP

#include <algorithm>
#include <cassert>
#include <numeric>
#include <vector>
#include "binary_search.hpp"

// ============================================================================
// 【并行二分查找 Parallel Binary Search (parallel_binary_search.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   template <typename Query, typename Integer, typename Reset, typename Advance, typename Check>
//   std::vector<Integer> parallel_binary_search(
//       const std::vector<Query>& queries, Integer low, Integer high,
//       Reset reset, Advance advance, Check check)
// 回调签名约定（在左闭右开时间域 [low, high) 上同时对 q 个询问二分）：
//   - `void reset()`                        ：每轮二分开始前重置全局数据结构；
//   - `void advance(Integer target_time)`   ：将数据结构单调推进到时刻 target_time；
//   - `bool check(const Query& q, Integer t)`：检验询问 q 在时刻 t 是否已满足条件。
//   返回长度为 q 的首个满足条件时刻数组（始终不满足则收敛为 high）。共 O(log(high - low)) 轮。
// ============================================================================

// template <typename Query, typename Integer, typename Reset, typename Advance, typename Check> std::vector<Integer> parallel_binary_search( — 功能：const std::vector<Query>& queries, Integer low, Integer high,
template <typename Query, typename Integer, typename Reset, typename Advance, typename Check>
std::vector<Integer> parallel_binary_search(const std::vector<Query>& queries,
	Integer low, Integer high, Reset reset, Advance advance, Check check) {
	assert(low <= high);
	const int n = queries.size();
	std::vector<Integer> left(n, low), right(n, high), mid(n);
	std::vector<int> active;
	while (true) {
		active.clear();
		for (int i = 0; i < n; ++i) {
			if (left[i] < right[i]) {
				mid[i] = binary_midpoint(left[i], right[i]);
				active.push_back(i);
			}
		}
		if (active.empty()) {
			return left;
		}
		std::sort(active.begin(), active.end(), [&](int a, int b) {
			return mid[a] < mid[b];
		});
		reset();
		for (const int id : active) {
			advance(mid[id]);
			if (check(queries[id], mid[id])) {
				right[id] = mid[id];
			} else {
				left[id] = mid[id] + 1;
			}
		}
	}
}

#endif
