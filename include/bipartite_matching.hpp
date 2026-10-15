#ifndef BIPARTITE_MATCHING_HPP
#define BIPARTITE_MATCHING_HPP

#include <algorithm>
#include <cassert>
#include <queue>
#include <utility>
#include <vector>

// ============================================================================
// 【Hopcroft-Karp 二分图最大匹配 (bipartite_matching.hpp)】
// ----------------------------------------------------------------------------
// 结果结构体签名：
//   struct BipartiteMatchingResult {
//       int size = 0;            // 最大匹配边数
//       std::vector<int> left;   // 长度为 left_size，left[u] 为左部点 u 匹配的右部点编号（未匹配为 -1）
//       std::vector<int> right;  // 长度为 right_size，right[v] 为右部点 v 匹配的左部点编号（未匹配为 -1）
//   };
// 求解函数签名：
//   BipartiteMatchingResult bipartite_matching(
//       const std::vector<std::vector<int>>& adj, int right_size)
// 参数与返回值详解：
//   - adj：左部点邻接表，左部点个数 left_size = adj.size()（0-based [0, left_size)），
//          adj[u] 存储左部点 u 连向的所有右部点编号 v（要求 0 <= v < right_size）。
//   - right_size：右部点总个数（0-based [0, right_size)，要求 right_size >= 0）。
//   - 返回值 BipartiteMatchingResult{size, left, right}。
// 复杂度：时间复杂度 O(E sqrt(V))，空间复杂度 O(V + E)。
// ============================================================================

struct BipartiteMatchingResult {
	int size = 0;
	std::vector<int> left, right;
};

BipartiteMatchingResult bipartite_matching(
		const std::vector<std::vector<int>>& adj, int right_size) {
	assert(right_size >= 0);
	const int left_size = adj.size();
	std::vector<int> left_match(left_size, -1), right_match(right_size, -1);
	std::vector<int> distance(left_size), current(left_size);
	int shortest;
	auto dfs = [&](auto&& self, int u) -> bool {
		for (int& i = current[u]; i < (int)adj[u].size(); ++i) {
			const int v = adj[u][i];
			assert(0 <= v && v < right_size);
			if ((right_match[v] == -1 && distance[u] + 1 == shortest) ||
				(right_match[v] != -1 && distance[right_match[v]] == distance[u] + 1 &&
				 self(self, right_match[v]))) {
				left_match[u] = v;
				right_match[v] = u;
				return true;
			}
		}
		distance[u] = -1;
		return false;
	};
	int size = 0;
	while (true) {
		std::fill(current.begin(), current.end(), 0);
		shortest = left_size + 1;
		std::queue<int> queue;
		for (int u = 0; u < left_size; ++u) {
			distance[u] = left_match[u] == -1 ? 0 : -1;
			if (distance[u] == 0) queue.push(u);
		}
		while (!queue.empty()) {
			const int u = queue.front(); queue.pop();
			if (distance[u] >= shortest) continue;
			for (const int v : adj[u]) {
				assert(0 <= v && v < right_size);
				if (right_match[v] == -1) shortest = distance[u] + 1;
				else if (distance[right_match[v]] == -1) {
					distance[right_match[v]] = distance[u] + 1;
					queue.push(right_match[v]);
				}
			}
		}
		int augmented = 0;
		for (int u = 0; u < left_size; ++u)
			if (left_match[u] == -1 && dfs(dfs, u)) ++augmented;
		if (!augmented) break;
		size += augmented;
	}
	return {size, std::move(left_match), std::move(right_match)};
}

#endif
