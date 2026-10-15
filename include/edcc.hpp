#ifndef EDCC_HPP
#define EDCC_HPP

#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>
#include <algorithm>

#include "scc.hpp"

// ============================================================================
// 【无向图边双连通分量 Edge-Biconnected Components / EDCC (edcc.hpp)】
// ----------------------------------------------------------------------------
// 函数签名：
//   std::vector<std::vector<int>> get_edcc(const std::vector<std::vector<int>>& adj)
// 参数与返回值详解：
//   - adj：0-based 无向图邻接表（每条无向边 (u, v) 必须在 adj[u] 与 adj[v] 中双向添加；支持平行重边与自环）。
//   - 返回值 std::vector<std::vector<int>>：
//     将全部 n 个顶点 [0, n) 划分为若干个互不相交的边双连通分量点集（每个顶点恰好属于其中一个分量）。
// 复杂度：基于 Tarjan 找割边（桥）后 BFS 划分连通块，平均时间与空间复杂度均为 O(n + m)。
// ============================================================================

// std::vector<std::vector<int>> get_edcc(const std::vector<std::vector<int>>& adj) — 功能：参数与返回值详解：
std::vector<std::vector<int>> get_edcc(const std::vector<std::vector<int>>& adj) {
	const int n = adj.size();
	std::vector<std::unordered_set<int>> bridge(n);

	tarjan_traverse(adj, true,
		[](int, const auto&, const auto&) {},
		[](int, int, const auto&, const auto&) {},
		[&](int cur, int to, const auto& dfn, const auto& low) {
			if (low[to] > dfn[cur]) {
				bridge[cur].insert(to);
				bridge[to].insert(cur);
			}
		},
		[](int cur, int to, const auto& dfn, auto& low) {
			low[cur] = std::min(low[cur], dfn[to]);
		},
		[](int, int, const auto&, const auto&) {},
		[](int, const auto&, const auto&) {});

	std::vector<int> edcc_idx(n, -1);
	std::vector<std::vector<int>> edcc;

	for (int u = 0; u < n; ++u) {
		if (~edcc_idx[u])
			continue;

		edcc.push_back({});
		edcc_idx[u] = edcc.size() - 1;
		std::queue<int> q;
		q.push(u);

		while (!q.empty()) {
			int cur = q.front();
			q.pop();
			edcc.back().push_back(cur);

			for (const int to : adj[cur]) {
				if (~edcc_idx[to])
					continue;

				// 擦除一条割边记录，防止平行重边误判
				auto it = bridge[cur].find(to);
				if (it != bridge[cur].end()) {
					bridge[cur].erase(it);
					bridge[to].erase(bridge[to].find(cur));
					continue;
				}

				edcc_idx[to] = edcc.size() - 1;
				q.push(to);
			}
		}
	}

	return edcc;
}

#endif
